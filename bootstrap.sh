sudo DEBIAN_FRONTEND=noninteractive apt-get install podman containers-storage docker-compose -y
sudo tee /etc/apt/sources.list.d/steam-stable.list <<'EOF'
deb [arch=amd64,i386 signed-by=/usr/share/keyrings/steam.gpg] https://repo.steampowered.com/steam/ stable steam
deb-src [arch=amd64,i386 signed-by=/usr/share/keyrings/steam.gpg] https://repo.steampowered.com/steam/ stable steam
EOF
sudo dpkg --add-architecture i386
sudo apt-get update
sudo DEBIAN_FRONTEND=noninteractive apt-get install -y \
  libgl1-mesa-dri:amd64 \
  libgl1-mesa-dri:i386 \
  libgl1-mesa-glx:amd64 \
  libgl1-mesa-glx:i386 \
  steam-launcher

# add to games group
sudo useradd -d /home/dst -g 60 -m -s /bin/bash -u 6969 dst

# clone the dst repo and put in /opt
sudo git clone https://github.com/mathielo/dst-dedicated-server.git /opt/dst-dedicated-server
sudo chown -R dst:games /opt/dst-dedicated-server

mkdir -p ~/.klei/DoNotStarveTogether
steamcmd +login anonymous +app_update 343050 validate +quit

# Caves has a soft link to the same file
cp ${HOME}/docs/modoverrides.lua ${HOME}/.klei/DoNotStarveTogether/MyDediServer
cp ${HOME}/docs/dedicated_server_mods_setup.lua ${HOME}/.klei/DoNotStarveTogether/MyDediServer
cp ${HOME}/docs/worldgenoverride.lua ${HOME}/.klei/DoNotStarveTogether/MyDediServer/Master/
