sur windows :
    PowerShell:
        wsl --install
    WSL:
        sudo apt-get update;
        mkdir  ~/raytracer/ && cp -r ./* ~/raytracer/ && cd ~/raytracer
        *installer les libs*


sur linux :
    *installer les libs*
    cmake . -B build

*installation des libs*
WSL / linux :
    sudo apt install cmake; sudo apt install libconfig++-dev; sudo apt install g++;
