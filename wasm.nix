pkgs:
pkgs.mkShell rec {
  name = "snek";

  packages = with pkgs; [ 
    pkgs.emscripten
    pkgs.gnumake
  ];
}
