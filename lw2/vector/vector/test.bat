@echo off

set PROGRAM="%~1"

rem �������� ��������� �� ��������� ������� �����
%PROGRAM% < test_data/empty.txt > %TEMP%\out.txt
fc /B %TEMP%\out.txt test_data/empty.txt
if ERRORLEVEL 1 goto err

rem �������� ��������� �� ��������� ����� �� �������� ������ ��������
%PROGRAM% < test_data/string-test.txt > %TEMP%\out.txt
fc /B %TEMP%\out.txt test_data/empty.txt
if ERRORLEVEL 1 goto err

rem ��������� ��������� ��������� � ������� ����� � ����������� ����������
%PROGRAM% < test_data/test-1.txt > %TEMP%\out.txt
fc /B %TEMP%\out.txt test_data/test-1-out.txt
if ERRORLEVEL 1 goto err

rem ��������� ��������� �� ������� ������� �������������� � ������� �������� � �����, �� ����������� ����
%PROGRAM% < test_data/test-2.txt > %TEMP%\out.txt
fc /B %TEMP%\out.txt test_data/test-2-out.txt
if ERRORLEVEL 1 goto err

echo All tests passed
exit /B 0

:err
echo Program testing failed
exit /B 1