local spriteManager = {}

spriteManager.liste_sprites = {}

spriteManager.CreeSprite = function()
  
  sprite = {}  
  table.insert(spriteManager.liste_sprites, sprite)
  
  sprite.anime = function(pDt)
  end
  
  sprite.deplace = function(pDt)
  end

  return sprite
  
end

return spriteManager