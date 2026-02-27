# SNS3 installation

> Refrence :
> - https://github.com/sns3/sns3-satellite

### Install dependency
> ```
>sudo apt update && sudo apt install -y git automake cmake qtbase5-dev python3-dev python3-pip libxml2-dev mercurial graphviz libgraphviz-dev
>sudo pip install --upgrade pip setuptools cppyy pygraphviz
>```

### Installation by bake
> ```
>cd ~
>mkdir workspace
>cd workspace
>git clone https://gitlab.com/nsnam/bake.git
>```
>**Output :**
>```
>正複製到 'bake'...
>remote: Enumerating objects: 2332, done.
>remote: Counting objects: 100% (368/368), done.
>remote: Compressing objects: 100% (97/97), done.
>remote: Total 2332 (delta 289), reused 341 (delta 271), pack-reused 1964 (from 1)
>接收物件中: 100% (2332/2332), 2.78 MiB | 4.94 MiB/s, 完成.
>處理 delta 中: 100% (1569/1569), 完成.
>```
### Set environment parameters
>```
>export BAKE_HOME=`pwd`
>export PATH=$PATH:$BAKE_HOME/build/bin
>export PYTHONPATH=$BAKE_HOME/build/lib
>export LD_LIBRARY_PATH=$BAKE_HOME/build/lib
>```
>```
>cd bake
>./bake.py configure -e ns-allinone-3.43
>./bake.py check
>```
>
>**Output :**
>```
>> Python3 - OK
>> GNU C++ compiler - OK
>> Git - OK
>> Tar tool - OK
>> Unzip tool - OK
>> Make - OK
>> CMake - OK
>> patch tool - OK
>
> Path searched for tools: /usr/local/sbin /usr/local/bin /usr/sbin /usr/bin /sbin /bin /usr/games /usr/local/games /snap/bin /snap/bin /build/bin >/home/kevin/build/bin /home/kevin/build/bin /home/kevin/workspace/build/bin bin /home/kevin/workspace/build/lib
>```
### Deploy ns3 by bake
>```
>./bake.py download
>```
>**Output :**
>```
>>> Searching for system dependency qt - OK
>>> Searching for system dependency g++ - OK
>>> Searching for system dependency cppyy - OK
>>> Searching for system dependency gi-cairo - OK
>>> Searching for system dependency gir-bindings - OK
>>> Searching for system dependency pygobject - OK
>>> Searching for system dependency pygraphviz - OK
>>> Searching for system dependency python3-dev - OK
>>> Searching for system dependency libxml2-dev - OK
>>> Downloading click-ns-3.37 - (Nothing to do, source directory already exists) - OK
>>> Searching for system dependency automake - OK
>>> Searching for system dependency cmake - OK
>>> Searching for system dependency mercurial - OK
>>> Downloading netanim-3.109 - (Nothing to do, source directory already exists) - OK
>>> Downloading BRITE - (Nothing to do, source directory already exists) - OK
>>> Downloading openflow-dev - (Nothing to do, source directory already exists) - OK
>>> Downloading ns-3.43 (target directory:ns-3.43) - (Nothing to do, source directory already exists) - OK
>```
### Configure ns3
>```
>cd source/ns-3.43
>./ns3 clean
>./ns3 configure --build-profile=debug --enable-examples --enable-tests
>```
>**Output :**
>```
>-- Configuring done
>-- Generating done
>-- Build files have been written to: /home/kevin/workspace/bake/source/ns-3.43/cmake-cache
>Finished executing the following commands:
>mkdir cmake-cache
>/usr/bin/cmake -S /home/kevin/workspace/bake/source/ns-3.43 -B /home/kevin/workspace/bake/source/ns-3.43/cmake-cache -DCMAKE_BUILD_TYPE=debug -DNS3_ASSERT=ON ->DNS3_LOG=ON -DNS3_WARNINGS_AS_ERRORS=ON -DNS3_NATIVE_OPTIMIZATIONS=OFF -DNS3_EXAMPLES=ON -DNS3_TESTS=ON -G Unix Makefiles --warn-uninitialized
>```
>---
### Get satellite/ traffic/ma modules
```
cd contrib
git clone https://github.com/sns3/sns3-satellite.git satellite
git clone https://github.com/sns3/traffic.git traffic
git clone https://github.com/sns3/stats.git magister-stats
```

Note :
> When retrieving the satellite, traffic and magister-stats modules, you should put them under the `ns-3.43/contrib/`  folder.
> You can do so by cloning them directly in this folder, extracting them here, copying the files afterwards or using symbolic links.
> Make sure all the repositories are using compatible versions. The best way to ensure that is to use the same tag on all repositories. For the latest release:

### Go back to `ns-3.43` :
```
cd ~/workspace/bake/source/ns-3.43

```

### Go into the `satellite` and switch to 3.43 :
```
cd ~/workspace/bake/source/ns-3.43/contrib/satellite
git checkout 3.43
```

### Go back to contrib and enter the `traffic` :

```
cd ../traffic
git checkout 3.43
```

### Go back to contrib again and enter the `magister-stats` :
```
cd ../magister-stats
git checkout 3.43
```
---
### Configure CMake and ask it to build NS-3
>#### It will automatically build all modules found in `contrib` : (It will cost some time, about 30~50mins)
>```
>cd ~/workspace/bake/source/ns-3.43
>./ns3 clean
>./ns3 configure --build-profile=optimized --enable-examples --enable-tests
>./ns3 build
>```
>
>**Output :**
>
>```
>Finished executing the following commands:
>/usr/bin/cmake --build /home/kevin/workspace/bake/source/ns-3.43/cmake-cache -j 7
>```
### Post-Compilation
>Once you compiled SNS-3 successfully, you will need an extra step before being able to run any simulation: download the data defining the reference scenario of >the simulation.
>These data are available as a separate repository and bundled as a submodule in SNS-3. You can download them afterwards in the `satellite` repository using:
>```
>cd contrib/satellite
>git submodule update --init --recursive
>```
>
>**Output :**
>```
>子模組 'data'（https://github.com/sns3/sns3-data.git）已對路徑 'data' 註冊
>正複製到 '/home/kevin/workspace/bake/source/ns-3.43/contrib/satellite/data'...
>子模組路徑「data」：已簽出「4200675fa10f3eb9ba3627bc664240a331be4ba9」
>```
>
### Testing SNS-3 (It will cost 30~45mins)
>```
>cd ~/workspace/bake/source/ns-3.43
>./test.py --no-build
>```
>
>**Output :**
>```
>868 of 868 tests passed (868 passed, 0 skipped, 0 failed, 0 crashed, 0 valgrind errors)
>```

### Running SNS-3
>A scenario can be launched as follows :
>```
>./ns3 run "ns3-program"
>```
>If command line arguments are needed, the command becomes :
>```
>./ns3 run "ns3-program" -- --arg1=value1 --arg2=value2
>```
>To list all the available command line arguments of a scenario, run :
>```
>./ns3 run "ns3-program" -- --PrintHelp
>```
>If debug mode is enabled, gdb can be used :
>```
>./ns3 run --gdb "ns3-program"
>```
---
### SNS-3 is delivered with several examples. Each one allows to demonstrate one or several functionalities of SNS-3. Statistics are generated in the `data/sims/<ns3-program>` folder.

### The main examples are:
- `sat-cbr-example.cc` : Simple example with CBR traffic.
  
- `sat-regeneration-example.cc`: Example to test several regeneration modes on satellite: transparent, physical, link or network. By default, all simulations use one transparent satellite.
  
- `sat-constellation-example.cc`: Example with LEO and GEO satellite constellations. ISLs are used to route packets between satellites, with static routing.
  
- `sat-vhts-example.cc`: Create a VHTS scenario (high throughputs and high link capacities)
  
- `sat-iot-example.cc`: Create an IoT scenario (low throughputs and low link capacities)
  
- `sat-logon-example.cc`: Use the logon functionality to log the UTs on the NCC. Traffic on return channels is not send before UT is logged.
  
- `sat-ncr-example.cc`: Use NCR synchronization between UTs and GWs. UT clock is generally cheap, and need to be resynchronized periodically by the NCC to correctly schedule sending of frames on return channel.
  
- `sat-lora-example.cc`: Create a scenario with Lora configuration, and on transparent satellite. Lora is a LPWAN protocol developed for IoT.
  
- `sat-lora-regenerative-example.cc`: Create a scenario with Lora configuration and regenerative satellites.



