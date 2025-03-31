# Stolen from here: https://github.com/nixvital/nix-based-cpp-starterkit

{ lib, llvmPackages_16, cmake, SDL2, SDL2_ttf, clang-tools, game }:
llvmPackages_16.stdenv.mkDerivation rec {
  pname = "snek";
  version = "0.1.0";

  src = ./.;

  nativeBuildInputs = [ cmake clang-tools ];
  buildInputs = [ SDL2 ];

  cmakeFlags = [ "-DENABLE_TESTING=OFF" "-DENABLE_INSTALL=ON" ]
    ++ lib.optional game "-DSNEK_ALGORITHM=ON";

  SDL2TTFDIR = "${SDL2_ttf}";
  SDL2DIR = "${SDL2.dev}";
}
