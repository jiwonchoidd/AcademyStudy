@echo off
:_loop
set /a a+=1
echo ������ %a%��° �����Դϴ�.
if %a%==10 goto _break
start /d "E:\Team36\TeamProject36\output\Debug" /b ClientAsyncSelect.exe
goto _loop
:_break
echo ������ �����׿�.
pause