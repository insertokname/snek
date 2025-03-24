{ mkShell, emscripten, cmake }:
mkShell rec {
  name = "snek";

  packages = [ emscripten cmake ];
}
