{
  inputs = {
    nixpkgs = {
      url = "github:NixOS/nixpkgs/nixpkgs-25.11-darwin";
    };
    treefmt-nix = {
      url = "github:numtide/treefmt-nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    flake-utils = {
      url = "github:numtide/flake-utils";
    };
  };

  outputs =
    {
      nixpkgs,
      treefmt-nix,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        treefmtEval = treefmt-nix.lib.evalModule pkgs {
          projectRootFile = "flake.nix";
          programs = {
            actionlint = {
              enable = true;
            };
            clang-format = {
              enable = true;
            };
            deadnix = {
              enable = true;
            };
            nixf-diagnose = {
              enable = true;
            };
            nixfmt = {
              enable = true;
            };
            statix = {
              enable = true;
            };
            yamlfmt = {
              enable = true;
            };
            zizmor = {
              enable = true;
            };
          };
        };
      in
      {
        devShells = {
          default = pkgs.mkShell {
            buildInputs = [
              pkgs.gcc
              pkgs.ninja
              pkgs.python3
            ];
          };
        };
        formatter = treefmtEval.config.build.wrapper;
        checks = {
          formatting = treefmtEval.config.build.check;
        };
      }
    );
}
