# Guide des plugins raytracer

## Qu'est-ce qu'un plugin ?

Un plugin est un objet qui hérite de **ILight** ou **IObject**. Il est compilé comme une bibliothèque partagée `.so` et chargé dynamiquement à la runtime.

## Structure du répertoire

```
plugins/
  <nom_de_l_objet>/
    fichier.cpp
    fichier.hpp
    README.md
    (pas de sous-dossiers !)
```

Le CMake génère une `.so` nommée : `raytracer_<nom_de_l_objet>.so`

## Includes

- Incluez `"ILight.hpp"` ou `"IObject.hpp"`
- Les fichiers d'en-tête du dossier `shared/` sont disponibles sans chemin relatif
- Ne pas utiliser de chemins relatifs

## Deux fonctions obligatoires (extern C)

### 1. `GetSoType()`

```cpp
extern "C" SoTypeEnum GetSoType(void) {
    return OBJECT;  // ou LIGHT si c'est une lumière
}
```

Retourne la valeur correspondante de l'enum `SoTypeEnum` (voir `shared/SoType.hpp`).

### 2. `GetObject()` ou `GetLight()`

```cpp
extern "C" IObject* GetObject(std::map<std::string, std::string> params) {
    return new MonObjet(params);
}
```

ou

```cpp
extern "C" ILight* GetLight(std::map<std::string, std::string> params) {
    return new MaLumiere(params);
}
```

## Paramètres de configuration

L'objet reçoit une map clé-valeur construite depuis le fichier `.cfg`.

### Exemple de config `.cfg` :

```
primitives: {
  spheres: [
    { x = 60; y = 5; z = 40; r = 25; color = { r = 255; g = 64; b = 64; }; }
  ]
}
```

### Map générée (avec notation pointée) :

| Clé | Valeur |
|-----|--------|
| `x` | `60` |
| `y` | `5` |
| `z` | `40` |
| `r` | `25` |
| `color.r` | `255` |
| `color.g` | `64` |
| `color.b` | `64` |

**Note :** Pour les objets imbriqués, utilisez la notation pointée (ex: `color.r`).

