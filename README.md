# Don't Starve Together - Dedicated Server

* To play on the Dont Starve Together server, start the game on Steam, then look for the server named &quot;wonderland&quot;
* Streaming live to twitch.tv/s1y_b0rg 
* Clone this repo onto your game server.
* Create a dedicated user for the game, such as `dst`.
  * make the repo clone the home directory of the new dst user.

## Install `direnv`

* We will use `direnv` to pick up the secret token from an environment variable.
  * [install direnv](https://direnv.net/docs/installation.html)

```sh
curl -sfL https://direnv.net/install.sh | bash 
eval "$(direnv hook bash)"
```

Now create a file called `.envrc` in this top level folder.
It should have one line in it like so:

```sh
export CLUSTER_TOKEN="pef-g^KU_QrGp3bke^uerRdBrRFyKING2q9zypelrosOjFYc1g="
```

## Edit config files

* Edit the [files in the top level `./saves` directory](https://github.com/devsecfranklin/game-server-dontstarvetogether/tree/main/saves) 
  * These will be copied into place by the scripts later.
* There is [a new script](saves/gen_mod_override.sh) to generate the `saves/modoverrides.lua` file. 
  * It just got tedious recreating it by hand every time we wanted to test a new server side mod.

## Crontab Entry

You can cause the gamse server to update itself at a time when you should probably be sleeping.
Use the `crontab -e` command to add this line to your dst service user crontab:

`0 4 * * * /usr/games/steamcmd +login anonymous +app_update 343050 validate +quit`

## Run Scripts

These two scripts set up the environment and start the game server.

```sh
./bin/bootstrap.sh
./bin/run_dedicated_server.sh
```

