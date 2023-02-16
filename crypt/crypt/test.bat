@echo off

set PROGRAM="%~1"

rem проверяем программу на неверное количество аргументов на входе
%PROGRAM% crypt
if NOT ERRORLEVEL 1 goto err
%PROGRAM% crypt test_data/hello-world.bin %TEMP%\out.txt 177 param
if NOT ERRORLEVEL 1 goto err

rem проверяем программу на неудачное открытие файла для чтения
%PROGRAM% crypt non-existing-file %TEMP%\out.txt 177
if NOT ERRORLEVEL 1 goto err

rem проверяем программу на неудачное открытие файла для записи
%PROGRAM% decrypt test_data/hello-world-crypt.bin %TEMP%\ 177
if NOT ERRORLEVEL 1 goto err

rem проверяем программу на неудачное открытие файла для записи файлом для чтения
%PROGRAM% decrypt test_data/hello-world-crypt.bin test_data/only-readable-file.txt 177
if NOT ERRORLEVEL 1 goto err

rem проверяем программу на некорректный параметр <action>
%PROGRAM% non-exist-action test_data/hello-world.bin %TEMP%\out.txt 177
if NOT ERRORLEVEL 1 goto err

rem проверяем параметр <key> программы на граничные значения
%PROGRAM% crypt test_data/hello-world.bin %TEMP%\out.txt -1
if NOT ERRORLEVEL 1 goto err
%PROGRAM% crypt test_data/hello-world.bin %TEMP%\out.txt 256
if NOT ERRORLEVEL 1 goto err

rem проверяем программу на некорректный параметр <key>
%PROGRAM% crypt test_data/hello-world.bin %TEMP%\out.txt string
if NOT ERRORLEVEL 1 goto err

rem проверяем программу на шифрование
%PROGRAM% crypt test_data/hello-world.bin %TEMP%\out.bin 177
fc %TEMP%\out.bin test_data/hello-world-crypt.bin
if ERRORLEVEL 1 goto err

rem проверяем программу на дешифрование
%PROGRAM% decrypt test_data/hello-world-crypt.bin %TEMP%\out.bin 177
fc %TEMP%\out.bin test_data/hello-world.bin
if ERRORLEVEL 1 goto err

echo All tests passed
exit /B 0

:err
echo Program testing failed
exit /B 1