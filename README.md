# ESP32 NVS CLI Configurator (`esp32-nvs-cli-config`)

Moduł zarządzania nieulotną pamięcią Flash wraz z interfejsem wiersza poleceń (CLI) przez UART dla mikrokontrolera ESP32. Program służy do trwałego przechowywania, odczytu oraz modyfikacji parametrów konfiguracyjnych rakiety pokładowej w przestrzeni nazw `rocket_cfg`. 

Projekt został zrealizowany w środowisku ESP-IDF z wykorzystaniem biblioteki NVS (dokumentacja pomocnicza: [ESP32_NVS](https://github.com/VPavlusha/ESP32_NVS)).

## Architektura projektu

* **`nvs_manager.c` / `.h`** — główny moduł odpowiedzialny za inicjalizację partycji NVS, zapis i odczyt danych (`int` jako `int32_t`, `float` jako `BLOB`, `bool` jako `uint8_t`, `string` jako `char*`), a także ich modyfikację i usuwanie. Zawiera również funkcję ładującą domyślne parametry startowe rakiety (`pulap`, `kat`, `uzbrojony`, `nazwa`).
* **`cli_manager.c` / `.h`** — parser poleceń terminala oparty na funkcji `strtok`. Rozpoznaje komendy (`set`, `get`, `mod`, `del`, `list`, `test`) i uruchamia przypisane do nich operacje.
* **`test_nvs.c` / `.h`** — zestaw 5 testów jednostkowych weryfikujących zapis, odczyt, nadpisywanie oraz usuwanie każdego obsługiwanego typu danych.
* **`main.c`** — główna pętla FreeRTOS obsługująca odbiór znaków przez UART.

## Jak działa pamięć NVS?

* **Zapis przez bufor RAM:** Funkcje `nvs_set_*` przygotowują dane w pamięci RAM, a dopiero `nvs_commit()` fizycznie utrwala je we Flashu (dane nie giną po odłączeniu zasilania).
* **Ochrona przed zużyciem (*wear leveling*):** Zamiast wielokrotnie kasować ten sam blok pamięci Flash, NVS dopisuje nowe wersje zmiennych w kolejnych wolnych miejscach i oznacza stare wpisy jako nieaktualne.

## Dostępne komendy CLI

| Komenda | Składnia | Opis | Przykład |
| :--- | :--- | :--- | :--- |
| **`set`** | `set <typ> <klucz> <wartosc>` | Tworzy nowy parametr | `set int limit 5000` |
| **`mod`** | `mod <typ> <klucz> <wartosc>` | Modyfikuje wartość istniejącego parametru | `mod float kat 90.0` |
| **`get`** | `get <typ> <klucz>` | Odczytuje pojedynczą wartość z pamięci Flash | `get bool uzbrojony` |
| **`del`** | `del <klucz>` | Usuwa wybrany klucz z pamięci Flash | `del limit` |
| **`list`** | `list` | Wyświetla wszystkie zapisane parametry w `rocket_cfg` | `list` |
| **`test`** | `test` | Uruchamia testy jednostkowe zapisu i odczytu NVS | `test` |

**Obsługiwane typy (`<typ>`):** `int`, `float`, `bool` (`true`/`false` lub `1`/`0`), `str`.

## Uruchomienie (ESP-IDF)

```bash
idf.py set-target esp32
idf.py build
idf.py -p <PORT> flash monitor
```

## Uwagi dla sprawdzających

> **Notatka autora:** W kodzie źródłowym znajduje się duża liczba własnoręcznie napisanych komentarzy — powstawały one na bieżąco w trakcie nauki obsługi pamięci NVS w ESP-IDF. Działanie całego systemu zostało przetestowane sprzętowo na płytce **ESP32 DevKit (38-pin WROOM)**.