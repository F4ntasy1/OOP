@echo off

set PROGRAM="%~1"

rem проверяем программу на неверное количество аргументов на входе
%PROGRAM%
if NOT ERRORLEVEL 1 goto err
%PROGRAM% 1 2
if NOT ERRORLEVEL 1 goto err

rem проверяем программу на граничные случаи для параметра <byte>
%PROGRAM% -1
if NOT ERRORLEVEL 1 goto err
%PROGRAM% 256
if NOT ERRORLEVEL 1 goto err

rem проверяем программу на некорректный параметр <byte>
%PROGRAM% string
if NOT ERRORLEVEL 1 goto err

rem проверяем программу на преобразование числа 6
%PROGRAM% 6 > %TEMP%\out.txt
fc %TEMP%\out.txt test_data/test_6.txt
if ERRORLEVEL 1 goto err

echo All tests passed
exit /B 0

:err
echo Program testing failed
exit /B 1