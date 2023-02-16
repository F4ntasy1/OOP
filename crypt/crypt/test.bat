@echo off

set PROGRAM="%~1"

rem ��������� ��������� �� �������� ���������� ���������� �� �����
%PROGRAM% crypt
if NOT ERRORLEVEL 1 goto err
%PROGRAM% crypt test_data/hello-world.bin %TEMP%\out.txt 177 param
if NOT ERRORLEVEL 1 goto err

rem ��������� ��������� �� ��������� �������� ����� ��� ������
%PROGRAM% crypt non-existing-file %TEMP%\out.txt 177
if NOT ERRORLEVEL 1 goto err

rem ��������� ��������� �� ��������� �������� ����� ��� ������
%PROGRAM% decrypt test_data/hello-world-crypt.bin %TEMP%\ 177
if NOT ERRORLEVEL 1 goto err

rem ��������� ��������� �� ��������� �������� ����� ��� ������ ������ ��� ������
%PROGRAM% decrypt test_data/hello-world-crypt.bin test_data/only-readable-file.txt 177
if NOT ERRORLEVEL 1 goto err

rem ��������� ��������� �� ������������ �������� <action>
%PROGRAM% non-exist-action test_data/hello-world.bin %TEMP%\out.txt 177
if NOT ERRORLEVEL 1 goto err

rem ��������� �������� <key> ��������� �� ��������� ��������
%PROGRAM% crypt test_data/hello-world.bin %TEMP%\out.txt -1
if NOT ERRORLEVEL 1 goto err
%PROGRAM% crypt test_data/hello-world.bin %TEMP%\out.txt 256
if NOT ERRORLEVEL 1 goto err

rem ��������� ��������� �� ������������ �������� <key>
%PROGRAM% crypt test_data/hello-world.bin %TEMP%\out.txt string
if NOT ERRORLEVEL 1 goto err

rem ��������� ��������� �� ����������
%PROGRAM% crypt test_data/hello-world.bin %TEMP%\out.bin 177
fc %TEMP%\out.bin test_data/hello-world-crypt.bin
if ERRORLEVEL 1 goto err

rem ��������� ��������� �� ������������
%PROGRAM% decrypt test_data/hello-world-crypt.bin %TEMP%\out.bin 177
fc %TEMP%\out.bin test_data/hello-world.bin
if ERRORLEVEL 1 goto err

echo All tests passed
exit /B 0

:err
echo Program testing failed
exit /B 1