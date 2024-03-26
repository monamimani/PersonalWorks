include_guard()

function(make_test_target_name resultTargetName targetName)
  set(${resultTargetName} ${${targetName}}Tests)
  return(PROPAGATE ${resultTargetName})
endfunction()

function(make_benchmark_target_name resultTargetName targetName)
  set(${resultTargetName} ${${targetName}}Benchmarks)
  return(PROPAGATE ${resultTargetName})
endfunction()
