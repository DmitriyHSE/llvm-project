define void @foo() {
  ret void
}
define void @bar() {
  call void @foo()
  call void @foo()
  ret void
}
