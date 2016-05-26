spriteManager = require("spritemanager")

sprite1 = spriteManager.CreeSprite()
sprite2 = spriteManager.CreeSprite()

print("Nombre de sprites",#spriteManager.liste_sprites)

local n
for n=1,#spriteManager.liste_sprites do
  print(n)
end
