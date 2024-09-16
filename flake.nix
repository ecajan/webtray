{
  description = "WebTray Flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      pkgs = import nixpkgs { system = "x86_64-linux"; };
    in
    {
      packages.x86_64-linux.default = pkgs.stdenv.mkDerivation {
        name = "webtray";
        src = self;
        buildPhase = "qmake6 . && make";
        installPhase = "mkdir -p $out/bin; install -t $out/bin webtray";

        buildInputs = [
          pkgs.kdePackages.wrapQtAppsHook
          pkgs.kdePackages.qmake
          pkgs.kdePackages.qtwebengine
          pkgs.kdePackages.qtwayland
        ];
      };
    };
}
