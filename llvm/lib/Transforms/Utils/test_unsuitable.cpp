define i32 @foo_with_return() {
  ret i32 42
}
define void @bar_with_arg(i32 %x) {
  ret void
}
define void @caller() {
  %a = call i32 @foo_with_return()
  call void @bar_with_arg(i32 5)
  ret void
}