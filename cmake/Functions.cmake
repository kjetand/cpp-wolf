# Copy files with a specific extension to a destination directory.
function(copy_files ext src dst)
  file(GLOB files ${src}/*.${ext})
  foreach (file ${files})
    message("-- Copy ${ext} ${file} to ${dst}")
    file(COPY ${file} DESTINATION ${dst})
  endforeach ()
endfunction()
