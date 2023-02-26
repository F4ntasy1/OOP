@echo off

set PROGRAM="%~1"

rem проверка программы на обработку пустого файла
%PROGRAM% < test_data/empty.txt > %TEMP%\out.txt
fc /B %TEMP%\out.txt test_data/empty.txt
if ERRORLEVEL 1 goto err

rem проверка программы на обработку файла со строками вместо значений
%PROGRAM% < test_data/string-test.txt > %TEMP%\out.txt
fc /B %TEMP%\out.txt test_data/empty.txt
if ERRORLEVEL 1 goto err

rem проверяем результат программы с помощью файла с допустимыми значениями
%PROGRAM% < test_data/test-1.txt > %TEMP%\out.txt
fc /B %TEMP%\out.txt test_data/test-1-out.txt
if ERRORLEVEL 1 goto err

rem проверяем программу на нулевое среднее арифметическое с помощью значений в файле, не превышающих нуля
%PROGRAM% < test_data/test-2.txt > %TEMP%\out.txt
fc /B %TEMP%\out.txt test_data/test-2-out.txt
if ERRORLEVEL 1 goto err

echo All tests passed
exit /B 0

:err
echo Program testing failed
exit /B 1