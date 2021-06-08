main:
  ld a, $45
  push a
  ld a, $00
  pop a
  end

; Expected output: 69