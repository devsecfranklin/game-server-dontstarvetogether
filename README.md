# Don't Starve Together - Dedicated Server

* To play on the Dont Starve Together server, start the game on Steam, then look for the server named &quot;wonderland&quot;
* Streaming live to twitch.tv/s1y_b0rg 
* Clone this repo onto your game server.
* Create a dedicated user for the game, such as `dst`.
  * make the repo clone the home directory of the new dst user.

## Install `direnv`

* We will use `direbv` to pick up the secret token from an environment variable.
  * [install direnv](https://direnv.net/docs/installation.html)

```sh
curl -sfL https://direnv.net/install.sh | bash 
eval "$(direnv hook bash)"
```

## Edit config files

* Edit the [files in the `saves` directory](https://github.com/devsecfranklin/game-server-dontstarvetogether/tree/main/saves) 
  * These will be copied into place by the scripts later.

## Run Scripts

These two scripts set up the envirronment and start the game server.

```sh
./bootstrap.sh
./run_dedicated_server.sh
```

