#include "OnnxRT/OnnxRTSession.h"

#include <format>
#include <iostream>

#include "Core/PathUtils.h"
#include "cpu_provider_factory.h"
#include "dml_provider_factory.h"
//#include "cuda_provider_factory.h"
//#include "tensorrt_provider_factory.h"
//
namespace OnnxRT
{
std::unique_ptr<OrtInstance> OrtInstance::m_instance{};

void OrtInstance::createOrtInstance(std::string_view onnxRTLogId)
{
  OrtInstance::m_instance = std::unique_ptr<OrtInstance>(new OrtInstance(std::string(onnxRTLogId)));
}

OrtInstance::OrtInstance(std::string onnRTLogId)
{
  m_env = std::make_unique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, onnRTLogId.c_str());
}

OrtSession::OrtSession(OrtSessionInfo info)
{
  Ort::SessionOptions session_options;
  session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

  int deviceId = 0;
  // device_id, also known as *the adapter index.The device ID corresponds to the enumeration order of hardware adapters as given by *IDXGIFactory::EnumAdapters.
  // A device_id of 0 always corresponds to the default adapter, which is typically the *primary display GPU installed on the system.

  switch (info.providerType)
  {
    case OrtProviderType::CPU:
    {
      OrtSessionOptionsAppendExecutionProvider_CPU(session_options, true);
    }
    break;
    case OrtProviderType::Cuda:
    {
      // OrtSessionOptionsAppendExecutionProvider_CUDA(session_options, deviceId);
    }
    break;
    case OrtProviderType::DirectML:
    {
      // DirectML doesn't support these
      session_options.DisableMemPattern();
      session_options.SetExecutionMode(ExecutionMode::ORT_SEQUENTIAL);

      OrtSessionOptionsAppendExecutionProvider_DML(session_options, deviceId);
    }
    break;
    default:
      break;
  }

  auto graphFilename = info.modelPath.filename().string();
  auto graphFilenameExtension = info.modelPath.extension().string();
  auto optimizedGraphName = std::format("{}Optimized.{}", graphFilename, graphFilenameExtension);
  auto optimizedGraphPath = getCacheGraphPath() / optimizedGraphName;

  auto isOptimizedGraphExist = doesCachedOptimizedGraphExists(optimizedGraphPath);

  auto graphToLoad = std::filesystem::path();
  if (!isOptimizedGraphExist)
  {
    graphToLoad = info.modelPath;

    std::filesystem::create_directories(optimizedGraphPath.parent_path());
    session_options.SetOptimizedModelFilePath(optimizedGraphPath.c_str());
  }
  else
  {
    graphToLoad = optimizedGraphPath;
  }

  m_session = Ort::Session(OrtInstance::getInstance()->getEnv(), graphToLoad.c_str(), session_options);
}

std::filesystem::path OrtSession::getCacheGraphPath()
{
  auto cachePath = getCachePath();
  auto optimizedGraphCachePath = cachePath / "Graph";
  return optimizedGraphCachePath;
}

bool OrtSession::doesCachedOptimizedGraphExists(std::filesystem::path cachedOptimizeGraphPath)
{
  return std::filesystem::exists(cachedOptimizeGraphPath);
}

void OrtSession::run(const std::vector<const char*>& outputNodesName)
{
  std::vector<const char*> inputNodesNamesCStr;

  for (auto& name : m_inputNodesNames)
  {
    inputNodesNamesCStr.push_back(name.c_str());
  }

  for (size_t i = 0; i < 10; i++)
  {
    std::format_to(std::ostream_iterator<char>(std::cout), "loop index {}\n", i);
    m_outputNodesValues.clear();
    {
      auto startTime = std::chrono::high_resolution_clock::now();
      // score model & input tensor, get back output tensor
      m_outputNodesValues = m_session.Run(Ort::RunOptions{nullptr}, inputNodesNamesCStr.data(), m_inputNodesValues.data(), m_inputNodesValues.size(), outputNodesName.data(), outputNodesName.size());
      assert(m_outputNodesValues.size() == 1 && m_outputNodesValues.front().IsTensor());
      auto endTime = std::chrono::high_resolution_clock::now();

      auto duration = endTime - startTime;

      std::cout << std::format("Session run time: {}ns, {}ms.\n", duration.count(), std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(duration).count());
    }
  }
}

} // namespace OnnxRT
