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

      overlays.default = final: prev: { inherit (self.packages.${prev.system}) webtray; };

      nixosModules.default =
        {
          pkgs,
          lib,
          config,
          ...
        }:
        {
          options.webtray.instances = lib.mkOption {
            type = lib.types.attrsOf (
              lib.types.submodule {
                options = {
                  url = lib.mkOption {
                    description = "URL of the WebTray instance";
                    type = lib.types.strMatching "http[s]?://[a-z0-9.]*:?[0-9]*";
                  };
                  autoStart = lib.mkOption {
                    description = "WebTray Instances to start on login";
                    type = lib.types.bool;
                    default = true;
                  };
                  openInWindow = lib.mkOption {
                    description = "Open Instance as Window";
                    type = lib.types.bool;
                    default = false;
                  };
                };
              }
            );
          };

          config = {
            environment.systemPackages = [ self.packages.${pkgs.system}.default ];

            systemd.user.services = builtins.mapAttrs (name: value: {
              enable = true;
              requires = if value.autoStart then [ "tray.target" ] else [ ];
              wantedBy = [ "graphical-session.target" ];
              description = "WebTray Instance for ${name}";
              serviceConfig = {
                Type = "simple";
                Restart = "on-failure";
                ExecStart = "${self.packages.${pkgs.system}.default}/bin/webtray ${value.url}${
                  if value.openInWindow then " --open-at-startup" else ""
                }";
              };
            }) config.webtray.instances;
          };
        };
    };
}
