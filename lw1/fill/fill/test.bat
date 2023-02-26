@echo off

set PROGRAM="%~1"

rem ��������� ��������� �� �������� ���������� ���������� �� �����
%PROGRAM% test_data/fill.txt
if NOT ERRORLEVEL 1 goto err
%PROGRAM% test_data/fill.txt %TEMP%\out.txt param
if NOT ERRORLEVEL 1 goto err

rem ��������� ��������� �� ��������� �������� ����� ��� ������
%PROGRAM% non-existing-file %TEMP%\out.txt
if NOT ERRORLEVEL 1 goto err

rem ��������� ��������� �� ��������� �������� ����� ��� ������
%PROGRAM% test_data/fill.txt test_data/only-readable-file.txt
if NOT ERRORLEVEL 1 goto err

rem ��������� ��������� �� ����������������� �� ����������
%PROGRAM% test_data/fill.txt %TEMP%\out.txt
fc %TEMP%\out.txt test_data/fill-out.txt
if ERRORLEVEL 1 goto err

rem ��������� ������ ��������� ��� ������ ������� �����
%PROGRAM% test_data/empty.txt out.txt
if ERRORLEVEL 1 goto err

echo All tests passed
exit /B 0

:err
echo Program testing failed
exit /B 1