
#include "OnnxRT/OnnxRTSession.h"



int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
//  OnnxRT::OrtInstance::createOrtInstance("OnnxInference");
//
//  auto graphName = "squeezenet";
//  auto graphExtension = "onnx";
//  auto fullGraphName = fmt::format("{}.{}", graphName, graphExtension);
//  auto dataPath = getDataPath();
//  auto modelsDir = dataPath / "Models";
//  auto modelPath = modelsDir / fullGraphName;
//
//  OnnxRT::OrtSession::OrtSessionInfo info;
//  info.dumpInfo = true;
//  info.modelPath = modelPath;
//
//  OnnxRT::OrtSession ortSession = OnnxRT::OrtSession(info);
//
//  auto fakeData = ortSession.tmpGetFakeData();
//  ortSession.setupInputs(fakeData);
//
//  ortSession.run({"softmaxout_1"});
//
  return EXIT_SUCCESS;
}
