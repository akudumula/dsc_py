gcc -O3 -o Proof_of_work Proof_of_work.c blake3.c blake3_dispatch.c blake3_portable.c     blake3_sse2_x86-64_windows_msvc.asm blake3_sse41_x86-64_windows_msvc.asm blake3_avx2_x86-64_windows_msvc.asm     blake3_avx512_x86-64_windows_msvc.asm -lpthread



gcc -O3 -o Proof_of_work Proof_of_work.c blake3.c blake3_dispatch.c blake3_portable.c     blake3_sse2_x86-64_windows_gnu.S blake3_sse41_x86-64_windows_gnu.S blake3_avx2_x86-64_windows_gnu.S     blake3_avx512_x86-64_windows_gnu.S -lpthread


gcc -O3 -o Proof_of_work Proof_of_work.c blake3.c blake3_dispatch.c blake3_portable.c     blake3_sse2_x86-64_unix.S blake3_sse41_x86-64_unix.S blake3_avx2_x86-64_unix.S     blake3_avx512_x86-64_unix.S -lpthread


python3 -m grpc_tools.protoc -I protos --python_out=. --grpc_python_out=. protos/pool.proto