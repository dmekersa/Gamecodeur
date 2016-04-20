local moduleAliens = {}

local aliens_mt = { __index = moduleAliens }

function moduleAliens.nouveau( )
		
  print("Module 4 / création d'une instance de moduleAliens")
  
	local nouveauModuleAliens = {}
  nouveauModuleAliens.nombre = 0
	
	return setmetatable( nouveauModuleAliens, aliens_mt )
end

function moduleAliens:AjouteAlien()
  self.nombre = self.nombre + 1
  
  print("Module 4 / Il y a maintenant",self.nombre,"aliens")
end

return moduleAliens