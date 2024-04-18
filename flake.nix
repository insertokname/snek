# in flake.nix
{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, flake-utils }:
    # calling a function from `flake-utils` that takes a lambda
    # that takes the system we're targetting
    flake-utils.lib.eachDefaultSystem (system:
      let
        name = "sdl-snek";
        pkgs = nixpkgs.legacyPackages.${system};
        src = ./.;
      in
      {
        # `eachDefaultSystem` transforms the input, our output set
        # now simply has `packages.default` which gets turned into
        # `packages.${system}.default` (for each system)
        
		packages.default = pkgs.stdenv.mkDerivation {
        
		inherit system name src;
        
		  
		  buildInputs = with pkgs; [
			bash
			coreutils
			gcc
			gnumake
			SDL2
		];

		unpackPhase = "true";

		installPhase = ''
			#ls $sdl/lib -lha > $out
			#ls > $out 
			#ls > $out
			#sdl2-config --libs > $out

			cp -r $src/src src
			cp $src/Makefile Makefile
			mkdir $out
			mkdir $out/bin
			chmod --recursive +w src
			chmod --recursive +w $out
			chmod --recursive +w $out/bin
			export TEMPOUT=$out
			make
			'';		
		};
      }
    );
}
   
