# Stolen from here: https://github.com/nixvital/nix-based-cpp-starterkit

{ lib, llvmPackages_16, cmake, SDL2, clang-tools }:
llvmPackages_16.stdenv.mkDerivation rec {
  pname = "snek";
  version = "0.1.0";

  src = ./.;

  nativeBuildInputs = [ cmake clang-tools ];
  buildInputs = [ SDL2 ];

  cmakeFlags = [ "-DENABLE_TESTING=OFF" "-DENABLE_INSTALL=ON" ];
}
