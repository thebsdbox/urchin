
# urchin

This is a tool to create the harsh environments "some" programs create inside of containers.

### Building

Once the repo is cloned move to the `dockerfile` directory and build the new image:

```
docker build --no-cache -t urchin .
```

This will compile urchin from a `gcc` image and then create a new image from an `ubuntu` image to run `urchin`. 


### Usage

To run `urchin` start the container through Docker.

**Allocate 1Gb of ram**

```
docker run -it --rm urchin:latest /urchin -m 1076277500
```

**Start web UI on port 8080**

```
docker run -it --rm urchin:latest /urchin -w 8080
```


**Create 10 forked zombies**

```
docker run -it --rm urchin:latest /urchin -e 10
```

### Current state

`Urchin` will automatically capture a number of Linux signals, when Docker attemptes to `stop` the container the `SIGTERM` signal is sent, this is captured and is ignored e.g.

`docker stop <container ID>`

.. inside container ..

`received SIGTERM`

Eventurally Docker will send a `SIGKILL`

**Help**

The following will display the available help/commands:

```
docker run -it --rm urchin:latest /urchin -h
```
