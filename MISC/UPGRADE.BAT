@echo Обновление игры Супер-Пух...
@if not exist mult.fml goto nom
@if not exist levels.fml goto nol
@if not exist graph.fml goto nog
fml mult ^zast.pic ^winny1.pic ^ia0.pic ^krolik0.pic
@echo Мультяшки и заставка обновлены...
fml levels ^01strip.def ^02strip.def ^01bckg.bin
@echo Уровни обновлены...
fml graph %%
@echo Обновление закончено.
@echo Удалите upgrade.bat
@goto end

:nog
@echo НЕТ файла graph.fml
@goto end

:nol
@echo НЕТ файла levels.fml
@goto end

:nom
@echo НЕТ файла mult.fml
@goto end

 11 ZAST.PIC      64000  32556 RLE  50
  1 01BCKG.BIN    64000  38182 RLE  59
 63 01STRIP.DEF     263    263 Нет 100
 64 02STRIP.DEF     218    217 RLE  99

  6 WINNY1.PIC    40120   7828 RLE  19
 14 IA0.PIC       51570  10908 RLE  21
 15 KROLIK0.PIC   61690  10391 RLE  16

:end