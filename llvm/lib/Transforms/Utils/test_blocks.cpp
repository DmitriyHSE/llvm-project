define void @foo(i1 %cond) {
  br i1 %cond, label %then, label %else
then:
  ret void
else:
  ret void
}
define void @bar(i1 %cond) {
  call void @foo(i1 %cond)
  ret void
}