

function on_uae_vsync()
   --io.write(uae_peek_symbol32("_hw_verticalBlankCount"), "\n");

   if frData == nill and uae_peek_symbol32("_hw_verticalBlankCount") == 2 then
      frData = io.open("lua.data", "w")
   end

   if frData ~= nil then
      frData:write(uae_peek_symbol16("_game_average"), "\n");
   end

end
