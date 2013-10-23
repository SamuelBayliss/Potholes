set (BENCHMARK_ARCHIVE polybench-c-3.2.tar)
set (BENCHMARK_MANIFEST benchmarks)


file(COPY ${BENCHMARK_ARCHIVE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${BENCHMARK_MANIFEST} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(benchmark)

set(LINEAR_KERNELS
2mm
3mm
atax
bicg
cholesky
doitgen
gemm
gemver
gesummv
mvt
symm
syr2k
syrk
trisolv
trmm
)

set(LINEAR_SOLVERS
durbin
dynprog
gramschmidt
lu
ludcmp
)
set(DATAMINING
correlation
covariance
)
set(MEDLEY
floyd-warshall
reg_detect
)

set(STENCILS 
adi
fdtd-2d
fdtd-apml
jacobi-1d-imper
jacobi-2d-imper
seidel-2d
)

add_definitions( -DPOLYBENCH_USE_C99_PROTO )
add_definitions( -DPOLYBENCH_USE_SCALAR_LB )


foreach(BENCHMARK ${LINEAR_KERNELS})
    list(APPEND BENCHMARKS "linear-algebra/kernels/${BENCHMARK}/${BENCHMARK}.c")
endforeach(BENCHMARK)

foreach(BENCHMARK ${LINEAR_SOLVERS})
    list(APPEND BENCHMARKS "linear-algebra/solvers/${BENCHMARK}/${BENCHMARK}.c")
endforeach(BENCHMARK)

foreach(BENCHMARK ${DATAMINING})
 list(APPEND BENCHMARKS "datamining/${BENCHMARK}/${BENCHMARK}.c")
endforeach(BENCHMARK)

foreach(BENCHMARK ${MEDLEY})
 list(APPEND BENCHMARKS "medley/${BENCHMARK}/${BENCHMARK}.c")
endforeach(BENCHMARK)

foreach(BENCHMARK ${STENCILS})
 list(APPEND BENCHMARKS "stencils/${BENCHMARK}/${BENCHMARK}.c")
endforeach(BENCHMARK)



foreach(BENCHMARK ${BENCHMARKS})
 list(APPEND FULL_PATH_BENCHMARKS "${CMAKE_CURRENT_BINARY_DIR}/${BENCHMARK}")
endforeach(BENCHMARK)

list(APPEND FULL_PATH_BENCHMARKS "${CMAKE_CURRENT_BINARY_DIR}/utilities/polybench.c")

add_custom_command(
OUTPUT ${FULL_PATH_BENCHMARKS}
COMMAND tar --strip-components=1 -xvf ${CMAKE_CURRENT_BINARY_DIR}/${BENCHMARK_ARCHIVE}
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)   

include_directories(${CMAKE_CURRENT_BINARY_DIR}/utilities)
add_library(polybench ${CMAKE_CURRENT_BINARY_DIR}/utilities/polybench)

foreach(BENCHMARK ${LINEAR_KERNELS})
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/utilities)
    add_executable(${BENCHMARK} ${CMAKE_CURRENT_BINARY_DIR}/linear-algebra/kernels/${BENCHMARK}/${BENCHMARK})
    target_link_libraries(${BENCHMARK} polybench)
endforeach(BENCHMARK)

foreach(BENCHMARK ${LINEAR_SOLVERS})
   include_directories(${CMAKE_CURRENT_BINARY_DIR}/utilities)
   add_executable(${BENCHMARK} linear-algebra/solvers/${BENCHMARK}/${BENCHMARK})
   target_link_libraries(${BENCHMARK} polybench)
endforeach(BENCHMARK)

foreach(BENCHMARK ${DATAMINING})
   include_directories(${CMAKE_CURRENT_BINARY_DIR}/utilities)
    add_executable(${BENCHMARK} datamining/${BENCHMARK}/${BENCHMARK})
    target_link_libraries(${BENCHMARK} polybench)
endforeach(BENCHMARK)

foreach(BENCHMARK ${MEDLEY})
   include_directories(${CMAKE_CURRENT_BINARY_DIR}/utilities)
    add_executable(${BENCHMARK} medley/${BENCHMARK}/${BENCHMARK})
    target_link_libraries(${BENCHMARK} polybench)
endforeach(BENCHMARK)

foreach(BENCHMARK ${STENCILS})
   include_directories(${CMAKE_CURRENT_BINARY_DIR}/utilities)
   add_executable(${BENCHMARK} ${CMAKE_CURRENT_BINARY_DIR}/stencils/${BENCHMARK}/${BENCHMARK})
   target_link_libraries(${BENCHMARK} polybench)
endforeach(BENCHMARK)
