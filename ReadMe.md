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
    sudo apt install cmake; sudo apt install libconfig++-dev;

---

## Système de Plugins (.so)

Le projet supporte la compilation automatique de plugins en tant que bibliothèques partagées (`.so`).

### Structure

```
plugins/
├── plugin1/
│   ├── Plugin1.cpp
│   ├── Plugin1.hpp (optionnel, dans le répertoire)
│   └── ... (autres fichiers)
├── plugin2/
│   ├── Plugin2.cpp
│   └── ...
└── raytracer_plugin1.so (généré)
```

### Comment créer un plugin

1. **Créer un dossier** dans `plugins/` avec le nom de ton plugin
2. **Ajouter des fichiers .cpp** contenant l'implémentation
3. **Utiliser les headers partagés** depuis `shared/` :
   - `shared/Light/` pour les lumières
   - `shared/Object/` pour les objets
   - `shared/Texture/` pour les textures

### Exemple

```cpp
// plugins/my_light/MyLight.cpp
#include "ALight.hpp"

class MyLight : public ALight {
    void Render() override {
        // implémentation
    }
};
```

### Compilation

```bash
cmake . -B build
cd build
make
```

Les plugins seront automatiquement compilés et placés dans `plugins/` avec le nom `raytracer_<nom_du_plugin>.so`

### Notes

- Chaque plugin a accès à tous les headers dans `shared/`
- Les compilations flags appliqués : `-Wall -Wextra -Werror`
