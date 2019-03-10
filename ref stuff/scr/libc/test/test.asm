
test.elf:     file format elf32-i386


Disassembly of section .text:

00100000 <init>:
  100000:	55                   	push   %ebp
  100001:	89 e5                	mov    %esp,%ebp
  100003:	83 ec 18             	sub    $0x18,%esp
  100006:	b8 03 00 00 00       	mov    $0x3,%eax
  10000b:	cd 80                	int    $0x80
  10000d:	c7 45 f0 00 00 00 00 	movl   $0x0,-0x10(%ebp)
  100014:	89 4d f0             	mov    %ecx,-0x10(%ebp)
  100017:	8b 45 f0             	mov    -0x10(%ebp),%eax
  10001a:	89 45 f4             	mov    %eax,-0xc(%ebp)
  10001d:	8b 45 f4             	mov    -0xc(%ebp),%eax
  100020:	ff d0                	call   *%eax
  100022:	83 ec 0c             	sub    $0xc,%esp
  100025:	50                   	push   %eax
  100026:	e8 17 00 00 00       	call   100042 <set_libc>
  10002b:	83 c4 10             	add    $0x10,%esp
  10002e:	83 ec 08             	sub    $0x8,%esp
  100031:	ff 75 0c             	pushl  0xc(%ebp)
  100034:	ff 75 08             	pushl  0x8(%ebp)
  100037:	e8 34 01 00 00       	call   100170 <main>
  10003c:	83 c4 10             	add    $0x10,%esp
  10003f:	90                   	nop
  100040:	c9                   	leave  
  100041:	c3                   	ret    

00100042 <set_libc>:
  100042:	55                   	push   %ebp
  100043:	89 e5                	mov    %esp,%ebp
  100045:	8b 45 08             	mov    0x8(%ebp),%eax
  100048:	8b 00                	mov    (%eax),%eax
  10004a:	a3 70 02 10 00       	mov    %eax,0x100270
  10004f:	8b 45 08             	mov    0x8(%ebp),%eax
  100052:	83 c0 04             	add    $0x4,%eax
  100055:	8b 00                	mov    (%eax),%eax
  100057:	a3 90 02 10 00       	mov    %eax,0x100290
  10005c:	8b 45 08             	mov    0x8(%ebp),%eax
  10005f:	83 c0 0c             	add    $0xc,%eax
  100062:	8b 00                	mov    (%eax),%eax
  100064:	a3 74 02 10 00       	mov    %eax,0x100274
  100069:	8b 45 08             	mov    0x8(%ebp),%eax
  10006c:	83 c0 10             	add    $0x10,%eax
  10006f:	8b 00                	mov    (%eax),%eax
  100071:	a3 98 02 10 00       	mov    %eax,0x100298
  100076:	8b 45 08             	mov    0x8(%ebp),%eax
  100079:	83 c0 14             	add    $0x14,%eax
  10007c:	8b 00                	mov    (%eax),%eax
  10007e:	a3 80 02 10 00       	mov    %eax,0x100280
  100083:	8b 45 08             	mov    0x8(%ebp),%eax
  100086:	83 c0 18             	add    $0x18,%eax
  100089:	8b 00                	mov    (%eax),%eax
  10008b:	a3 a0 02 10 00       	mov    %eax,0x1002a0
  100090:	8b 45 08             	mov    0x8(%ebp),%eax
  100093:	83 c0 1c             	add    $0x1c,%eax
  100096:	8b 00                	mov    (%eax),%eax
  100098:	a3 6c 02 10 00       	mov    %eax,0x10026c
  10009d:	8b 45 08             	mov    0x8(%ebp),%eax
  1000a0:	83 c0 20             	add    $0x20,%eax
  1000a3:	8b 00                	mov    (%eax),%eax
  1000a5:	a3 b8 02 10 00       	mov    %eax,0x1002b8
  1000aa:	8b 45 08             	mov    0x8(%ebp),%eax
  1000ad:	83 c0 24             	add    $0x24,%eax
  1000b0:	8b 00                	mov    (%eax),%eax
  1000b2:	a3 c0 02 10 00       	mov    %eax,0x1002c0
  1000b7:	8b 45 08             	mov    0x8(%ebp),%eax
  1000ba:	83 c0 28             	add    $0x28,%eax
  1000bd:	8b 00                	mov    (%eax),%eax
  1000bf:	a3 7c 02 10 00       	mov    %eax,0x10027c
  1000c4:	8b 45 08             	mov    0x8(%ebp),%eax
  1000c7:	83 c0 2c             	add    $0x2c,%eax
  1000ca:	8b 00                	mov    (%eax),%eax
  1000cc:	a3 a8 02 10 00       	mov    %eax,0x1002a8
  1000d1:	8b 45 08             	mov    0x8(%ebp),%eax
  1000d4:	83 c0 30             	add    $0x30,%eax
  1000d7:	8b 00                	mov    (%eax),%eax
  1000d9:	a3 78 02 10 00       	mov    %eax,0x100278
  1000de:	8b 45 08             	mov    0x8(%ebp),%eax
  1000e1:	83 c0 34             	add    $0x34,%eax
  1000e4:	8b 00                	mov    (%eax),%eax
  1000e6:	a3 a4 02 10 00       	mov    %eax,0x1002a4
  1000eb:	8b 45 08             	mov    0x8(%ebp),%eax
  1000ee:	83 c0 38             	add    $0x38,%eax
  1000f1:	8b 00                	mov    (%eax),%eax
  1000f3:	a3 bc 02 10 00       	mov    %eax,0x1002bc
  1000f8:	8b 45 08             	mov    0x8(%ebp),%eax
  1000fb:	83 c0 3c             	add    $0x3c,%eax
  1000fe:	8b 00                	mov    (%eax),%eax
  100100:	a3 88 02 10 00       	mov    %eax,0x100288
  100105:	8b 45 08             	mov    0x8(%ebp),%eax
  100108:	83 c0 40             	add    $0x40,%eax
  10010b:	8b 00                	mov    (%eax),%eax
  10010d:	a3 8c 02 10 00       	mov    %eax,0x10028c
  100112:	8b 45 08             	mov    0x8(%ebp),%eax
  100115:	83 c0 44             	add    $0x44,%eax
  100118:	8b 00                	mov    (%eax),%eax
  10011a:	a3 94 02 10 00       	mov    %eax,0x100294
  10011f:	8b 45 08             	mov    0x8(%ebp),%eax
  100122:	83 c0 48             	add    $0x48,%eax
  100125:	8b 00                	mov    (%eax),%eax
  100127:	a3 ac 02 10 00       	mov    %eax,0x1002ac
  10012c:	8b 45 08             	mov    0x8(%ebp),%eax
  10012f:	83 c0 4c             	add    $0x4c,%eax
  100132:	8b 00                	mov    (%eax),%eax
  100134:	a3 9c 02 10 00       	mov    %eax,0x10029c
  100139:	8b 45 08             	mov    0x8(%ebp),%eax
  10013c:	83 c0 50             	add    $0x50,%eax
  10013f:	8b 00                	mov    (%eax),%eax
  100141:	a3 b0 02 10 00       	mov    %eax,0x1002b0
  100146:	8b 45 08             	mov    0x8(%ebp),%eax
  100149:	83 c0 54             	add    $0x54,%eax
  10014c:	8b 00                	mov    (%eax),%eax
  10014e:	a3 c4 02 10 00       	mov    %eax,0x1002c4
  100153:	8b 45 08             	mov    0x8(%ebp),%eax
  100156:	83 c0 58             	add    $0x58,%eax
  100159:	8b 00                	mov    (%eax),%eax
  10015b:	a3 b4 02 10 00       	mov    %eax,0x1002b4
  100160:	8b 45 08             	mov    0x8(%ebp),%eax
  100163:	83 c0 5c             	add    $0x5c,%eax
  100166:	8b 00                	mov    (%eax),%eax
  100168:	a3 84 02 10 00       	mov    %eax,0x100284
  10016d:	90                   	nop
  10016e:	5d                   	pop    %ebp
  10016f:	c3                   	ret    

00100170 <main>:
  100170:	8d 4c 24 04          	lea    0x4(%esp),%ecx
  100174:	83 e4 f0             	and    $0xfffffff0,%esp
  100177:	ff 71 fc             	pushl  -0x4(%ecx)
  10017a:	55                   	push   %ebp
  10017b:	89 e5                	mov    %esp,%ebp
  10017d:	53                   	push   %ebx
  10017e:	51                   	push   %ecx
  10017f:	83 ec 10             	sub    $0x10,%esp
  100182:	89 cb                	mov    %ecx,%ebx
  100184:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%ebp)
  10018b:	eb 2b                	jmp    1001b8 <main+0x48>
  10018d:	a1 74 02 10 00       	mov    0x100274,%eax
  100192:	8b 55 f4             	mov    -0xc(%ebp),%edx
  100195:	8d 0c 95 00 00 00 00 	lea    0x0(,%edx,4),%ecx
  10019c:	8b 53 04             	mov    0x4(%ebx),%edx
  10019f:	01 ca                	add    %ecx,%edx
  1001a1:	8b 12                	mov    (%edx),%edx
  1001a3:	83 ec 04             	sub    $0x4,%esp
  1001a6:	52                   	push   %edx
  1001a7:	ff 75 f4             	pushl  -0xc(%ebp)
  1001aa:	68 5c 02 10 00       	push   $0x10025c
  1001af:	ff d0                	call   *%eax
  1001b1:	83 c4 10             	add    $0x10,%esp
  1001b4:	83 45 f4 01          	addl   $0x1,-0xc(%ebp)
  1001b8:	8b 45 f4             	mov    -0xc(%ebp),%eax
  1001bb:	3b 03                	cmp    (%ebx),%eax
  1001bd:	7c ce                	jl     10018d <main+0x1d>
  1001bf:	b8 00 00 00 00       	mov    $0x0,%eax
  1001c4:	8d 65 f8             	lea    -0x8(%ebp),%esp
  1001c7:	59                   	pop    %ecx
  1001c8:	5b                   	pop    %ebx
  1001c9:	5d                   	pop    %ebp
  1001ca:	8d 61 fc             	lea    -0x4(%ecx),%esp
  1001cd:	c3                   	ret    
