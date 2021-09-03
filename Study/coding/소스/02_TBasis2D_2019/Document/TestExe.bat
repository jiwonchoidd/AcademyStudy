@echo off
:_loop
set /a a+=1
echo 지금은 %a%번째 루프입니다.
if %a%==10 goto _break
start /d "E:\Team36\TeamProject36\output\Debug" /b ClientAsyncSelect.exe
goto _loop
:_break
echo 루프가 끝났네요.
pause