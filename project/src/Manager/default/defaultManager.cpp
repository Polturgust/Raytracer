/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** defaultManager.cpp
*/

#include "DefaultManager.hpp"

namespace raytracer {

void DefaultManager::Update(const std::vector<IObject> objects, const std::vector<ILight> lights, std::vector<std::vector<Tile>>& map) {
    _state = WORKING;
    if (_multiThread.isEnd()) {
        if (CalculingRow == CalculatedRow && CalculatedRow != 0)
            _Printer.ClearNbLine(2);
        else
            _Printer.ClearNbLine(1);
        int nb = _multiThread.GetTreadNumber();
        int end = CalculatedRow + nb > map.size() ? map.size() : CalculatedRow + nb;
        _multiThread.Compute(objects, lights, map, CalculatedRow, end, CalculingRow);
        CalculatedRow = end;
        _Printer.PrintLine(std::to_string((CalculingRow * 100) / map.size()) + "% Calculated.");
    } else {
        _Printer.ClearNbLine(1);
        _Printer.PrintLine(std::to_string((CalculingRow * 100) / map.size()) + "% Calculated.");
    }
    if (CalculingRow == map.size()) {
        _state = FINISH;
        return;
    }
}

}
