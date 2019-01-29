
setlocal enableextensions disabledelayedexpansion

set "search=sample"
set "replace=%1"

set "textFile=sample.vcxproj"

for /f "delims=" %%i in ('type "%textFile%" ^& break ^> "%textFile%" ') do (
 set "line=%%i"
 setlocal enabledelayedexpansion
 >>"%textFile%" echo(!line:%search%=%replace%!
 endlocal
)
rename sample.vcxproj %1.vcxproj
cd ..
rensamplefolder.bat %1