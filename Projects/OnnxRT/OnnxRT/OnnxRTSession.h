#pragma once

#include "OnnxRT/DllDefines.h"
#include "onnxruntime_cxx_api.h"

#include <cassert>
#include <filesystem>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

namespace OnnxRT
{
class ONNXRT_API OrtInstance
{
public:
  static void createOrtInstance(std::string_view onnRTLogId);

  static OrtInstance* getInstance()
  {
    return m_instance.get();
  }

  Ort::Env& getEnv()
  {
    return *m_env;
  }

private:
  OrtInstance(std::string onnRTLogId);

  static std::unique_ptr<OrtInstance> m_instance;

  std::unique_ptr<Ort::Env> m_env;
};

enum class ONNXRT_API OrtProviderType
{
    CPU,
    DirectML,
    Cuda,
};

class ONNXRT_API OrtSession
{
public:
  struct OrtSessionInfo
  {
    OrtProviderType providerType = OrtProviderType::CPU;
    std::filesystem::path modelPath;
    bool dumpInfo = false;
  };

  OrtSession(OrtSessionInfo info);

  template <typename T>
  void setupInputs(std::vector<std::pair<std::string, std::vector<T>>> inputNodesValues);

  template <typename T>
  void setupOutputs();

  template <typename T>
  T* getOutput(size_t index)
  {
    return m_outputNodesValues[index].GetTensorMutableData<T>();
  }

  std::vector<std::pair<std::string, std::vector<float>>> tmpGetFakeData()
  {
    Ort::TypeInfo type_info = m_session.GetInputTypeInfo(0);
    auto tensor_info = type_info.GetTensorTypeAndShapeInfo();

    Ort::AllocatorWithDefaultOptions allocator;

    char* input_name = m_session.GetInputName(0, allocator);

    size_t input_tensor_size = tensor_info.GetElementCount();
    std::vector<float> input_tensor_values(input_tensor_size);

    for (unsigned int i = 0; i < input_tensor_size; i++)
      input_tensor_values[i] = (float)i / (input_tensor_size + 1);

    return {{input_name, input_tensor_values}};
  }

  void run(const std::vector<const char*>& outputNodesName);

private:
  static std::filesystem::path getCacheGraphPath();
  static bool doesCachedOptimizedGraphExists(std::filesystem::path cachedOptimizeGraphPath);

  Ort::Session m_session = Ort::Session{nullptr};

  std::vector<std::string> m_inputNodesNames;
  std::vector<Ort::Value> m_inputNodesValues;
  std::vector<Ort::Value> m_outputNodesValues;
};

template <typename T>
void OrtSession::setupInputs(std::vector<std::pair<std::string, std::vector<T>>> inputNodesValues)
{
  int i = 0;
  for (auto& pair : inputNodesValues)
  {
    auto nodeName = pair.first;
    m_inputNodesNames.push_back(pair.first);

    Ort::TypeInfo inputTypeInfo = m_session.GetInputTypeInfo(i);
    auto tensorInfo = inputTypeInfo.GetTensorTypeAndShapeInfo();

    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(memory_info, pair.second.data(), tensorInfo.GetElementCount(), tensorInfo.GetShape().data(), 4);
    assert(inputTensor.IsTensor());

    m_inputNodesValues.push_back(std::move(inputTensor));

    i++;
  }

  //const char* const output_names[] = {"Label0", "F20", "F11"};
  //std::vector<Ort::Value> ort_outputs = session.Run(Ort::RunOptions{nullptr}, input_names.data(), ort_inputs.data(), ort_inputs.size(), output_names, countof(output_names));
}

template <typename T>
void OrtSession::setupOutputs()
{
}

} // namespace OnnxRT