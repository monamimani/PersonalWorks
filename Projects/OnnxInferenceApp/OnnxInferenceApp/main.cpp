
#include "OnnxRT/OnnxRTSession.h"
#include "Core/PathUtils.h"
#include <format>

//#pragma comment(linker, "/manifestdependency:\"type='win32' name='Lib.OnnxRT' version='0.0.0.1'\"")


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
  OnnxRT::OrtInstance::createOrtInstance("OnnxInference");

  auto graphName = "squeezenet";
  auto graphExtension = "onnx";
  auto fullGraphName = std::format("{}.{}", graphName, graphExtension);
  auto dataPath = getDataPath();
  auto modelsDir = dataPath / "AIModels";
  auto modelPath = modelsDir / fullGraphName;

  OnnxRT::OrtSession::OrtSessionInfo info;
  info.dumpInfo = true;
  info.modelPath = modelPath;

  OnnxRT::OrtSession ortSession = OnnxRT::OrtSession(info);

  auto fakeData = ortSession.tmpGetFakeData();
  ortSession.setupInputs(fakeData);

  ortSession.run({"softmaxout_1"});

  return EXIT_SUCCESS;
}
