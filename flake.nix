{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachSystem [
      "x86_64-linux"
      "i686-linux"
      "aarch64-linux"
      "aarch64-darwin"
      "x86_64-darwin"
    ] (system:
      let
        name = "snek";
        pkgs = nixpkgs.legacyPackages.${system};
        src = ./.;
      in {
        packages.game = pkgs.callPackage ./default.nix { game = false; };
        packages.algorithm = pkgs.callPackage ./default.nix { game = true; };
        devShells.wasm = pkgs.callPackage ./wasm.nix { };
      });
}

