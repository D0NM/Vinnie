@echo off
@echo Создание ДЕМО-версии
if not exist pooh.exe goto err0
if not exist graph.fml goto err1
if not exist levels.fml goto err2
if not exist mult.fml goto err3

del pooh.exe
ren demopooh.dem demopooh.exe

rem graph.fml
fml graph -WEBBER.BIN -STONEMEN.BIN -BAT.BIN -SLIME.BIN -PIG.BIN -GRSLIM.BIN
fml graph -ELSNAKE.BIN -SLONPOT.BIN -GLAZ.BIN -HAND.BIN -MEAT.BIN -SMEN.BIN
fml graph -SHARK.BIN -SEACAT.BIN -SNOWMEN.BIN -GHOST.BIN -RAT.BIN -RAMA03.BIN
fml graph -RAMA05.BIN -RAMA06.BIN -RAMA07.BIN -RAMA04.BIN

rem levels.fml
fml levels -03BCKG.BIN -03FON.BIN -03level.bin -03blevel.bin -03models.bin
fml levels -03monstr.bin -03def.bin -04def.bin -04level.bin -04blevel.bin
fml levels -04monstr.bin -04models.bin -05FON.BIN -05def.bin -05level.bin
fml levels -05blevel.bin -05monstr.bin -05models.bin -05BCKG.BIN -04BCKG.BIN
fml levels -07def.bin -07level.bin -07blevel.bin -07monstr.bin -07models.bin
fml levels -07FON.BIN -07BCKG.BIN -06def.bin -06level.bin -06blevel.bin
fml levels -06monstr.bin -06models.bin -06FON.BIN -06BCKG.BIN -03demo.bin
fml levels -04demo.bin -05demo.bin -06demo.bin -07demo.bin -03MONSTR.DEF
fml levels -04MONSTR.DEF -05MONSTR.DEF -07MONSTR.DEF -06MONSTR.DEF -03STRIP.DEF
fml levels -04STRIP.DEF -05STRIP.DEF -06STRIP.DEF -07STRIP.DEF

rem mult.fml
fml mult -DARK0.PIC -DARK1.PIC -DARK2.PIC -ESLPT0.PIC -ESLPT00.PIC -ESLPT1.PIC
fml mult -ZAVOD3.PIC -ZAVOD2.PIC -ZAVOD1.PIC -SEVPOL.PIC -ZAVOD4.PIC -EPIC1.PIC
fml mult -EPIC2.PIC -EPIC0.PIC -EFALPAT.PIC -ROBOT0.PIC -ROBOT1.PIC -SMOST0.PIC
fml mult -SMOST1.PIC -TRUBA.PIC -LES.PIC -KIT.PIC -ROBOTS.PIC -STLEV3.REC -STLEV4.REC
fml mult -ENDLEV3.REC -STLEV5.REC -ENDLEV5.REC -STLEV6.REC -ENDLEV4.REC
fml mult -ENDLEV6.REC -STLEV7.REC -BIKFORD.BIN -POLET.MUL

rem удаляем лишнюю музыку ку!
del COMING.FMS
del ICE.FMS   
del GLOOM.FMS 
del LEAF.FMS  
del LYRA.FMS  
del SIMPLE.FMS

goto end

:err0
echo Нет файла pooh.exe
@goto end

:err1
echo Нет файла graph.fml
@goto end

:err2
echo Нет файла levels.fml
@goto end

:err3
echo Нет файла mult.fml
@goto end

:end