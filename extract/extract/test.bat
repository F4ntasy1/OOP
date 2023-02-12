@echo off

set PROGRAM="%~1"

rem проверяем извлечение всего содержимого файла
%PROGRAM% test_data/hello-world.bin %TEMP%\hello-world.txt 0 12
fc.exe %TEMP%\hello-world.txt test_data/hello-world.bin
if ERRORLEVEL 1 goto err

rem проверяем извлечение пустого фрагмента
%PROGRAM% test_data/empty.bin %TEMP%\empty.txt 0 0
fc.exe %TEMP%\empty.txt test_data/empty.bin
if ERRORLEVEL 1 goto err

rem проверяем программу на неверное количество аргументов на входе
%PROGRAM% test_data/hello-world.bin %TEMP%\hello-world.txt 0
if NOT ERRORLEVEL 1 goto err
%PROGRAM% test_data/hello-world.bin %TEMP%\hello-world.txt 0 5 4
if NOT ERRORLEVEL 1 goto err

rem проверяем выход за границы файла для параметра <fragment size>
%PROGRAM% test_data/hello-world.bin %TEMP%\hello-world.txt 0 13
if NOT ERRORLEVEL 1 goto err

rem проверяем программу на некорректный параметр <fragment size>
%PROGRAM% test_data/hello-world.bin %TEMP%\hello-world.txt 0 -1
if NOT ERRORLEVEL 1 goto err
%PROGRAM% test_data/hello-world.bin %TEMP%\hello-world.txt 0 string
if NOT ERRORLEVEL 1 goto err

rem проверяем программу на некорректный параметр <start position>
%PROGRAM% test_data/hello-world.bin %TEMP%\hello-world.txt -1 0
if NOT ERRORLEVEL 1 goto err
%PROGRAM% test_data/hello-world.bin %TEMP%\hello-world.txt string 0
if NOT ERRORLEVEL 1 goto err

rem проверяем на неудачное открытие входного файла
%PROGRAM% non-existing-file.txt %TEMP%\out.txt 0 0
if NOT ERRORLEVEL 1 goto err

rem проверяем программу на неудачное открытие файла для записи
%PROGRAM% hello-world.bin %TEMP%\ 0 0
if NOT ERRORLEVEL 1 goto err

echo All tests passed
exit /B 0

:err
echo Program testing failed
exit /B 1