@echo off

set PROGRAM="%~1"

rem проверяем программу на неверное количество аргументов на входе
%PROGRAM% test_data/test-matrix1.txt
if NOT ERRORLEVEL 1 goto err
%PROGRAM% test_data/test-matrix1.txt test_data/test-matrix2.txt test-param
if NOT ERRORLEVEL 1 goto err

rem проверяем программу на неудачное открытие файлов для чтения
%PROGRAM% non-existing-file test_data/test-matrix1.txt
if NOT ERRORLEVEL 1 goto err
%PROGRAM% test_data/test-matrix1.txt non-existing-file
if NOT ERRORLEVEL 1 goto err

rem проверяем случай неподдерживаемых коэффициентов матриц
%PROGRAM% test_data/test-matrix-string-coeff.txt test_data/test-matrix2.txt
if NOT ERRORLEVEL 1 goto err
%PROGRAM% test_data/test-matrix2.txt test_data/test-matrix-string-coeff.txt
if NOT ERRORLEVEL 1 goto err

rem проверяем обработку пустого входного файла
%PROGRAM% test_data/empty.txt test_data/test-matrix1.txt
if NOT ERRORLEVEL 1 goto err
%PROGRAM% test_data/test-matrix1.txt test_data/empty.txt
if NOT ERRORLEVEL 1 goto err

set OUT="/matrix-result.txt"

rem проверяем программу на перемножение матриц
%PROGRAM% test_data/test-matrix1.txt test_data/test-matrix2.txt
fc %OUT% test_data/matrixResult.txt
if ERRORLEVEL 1 goto err

echo All tests passed
exit /B 0

:err
echo Program testing failed
exit /B 1