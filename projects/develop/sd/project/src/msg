on
platform pcspkr: uevent: unsupported action-string; this will be ignored in a future kernel version
vga16fb vga16fb.0: uevent: unsupported action-string; this will be ignored in a future kernel version
Linux agpgart interface v0.102
agpgart: Detected an Intel 440BX Chipset.
agpgart: AGP aperture is 64M @ 0xec000000
pcnet32.c:v1.34 14.Aug.2007 tsbogend@alpha.franken.de
pcnet32: PCnet/PCI II 79C970A at 0x1400, 00 0c 29 bd 56 18 assigned IRQ 18.
eth0: registered as PCnet/PCI II 79C970A
pcnet32: 1 cards_found.
eth0: link up
parport0: PC-style at 0x378 [PCSPP,TRISTATE,EPP]
lp0: using parport0 (polling).
Adding 208804k swap on /dev/hda5.  Priority:-1 extents:1 across:208804k
EXT3 FS on hda1, internal journal
NET: Registered protocol family 17
NET: Registered protocol family 10
kjournald starting.  Commit interval 5 seconds
EXT3-fs warning: maximal mount count reached, running e2fsck is recommended
EXT3 FS on hdb1, internal journal
EXT3-fs: mounted filesystem with ordered data mode.
eth0: no IPv6 routers present
ppdev: user-space parallel port driver
BUG: unable to handle kernel NULL pointer dereference at virtual address 00000014
printing eip: c010e90b *pde = 00000000 
Oops: 0000 [#1] 
Modules linked in: ppdev ipv6 af_packet md_mod parport_pc lp parport pcnet32 mii intel_agp agpgart evdev ext3 jbd mbcache ide_cd cdrom ide_disk piix ide_core unix vga16fb cfbcopyarea vgastate cfbimgblt cfbfillrect

Pid: 6320, comm: casio_task Not tainted (2.6.24-casio #1)
EIP: 0060:[<c010e90b>] EFLAGS: 00210002 CPU: 0
EIP is at insert_casio_task_rb_tree+0x11/0x53
EAX: 00000004 EBX: dfa3b1c0 ECX: 00000000 EDX: c06dd0e0
ESI: 000002c8 EDI: dfa3b1c0 EBP: c06dd0e0 ESP: dedf9d8c
 DS: 007b ES: 007b FS: 0000 GS: 0033 SS: 0068
Process casio_task (pid: 6320, ti=dedf8000 task=dede2fc0 task.ti=dedf8000)
Stack: dfa3b1c0 dede2fc0 c06dd0e0 00000001 c010fe42 c06dd540 de893960 dede2fc0 
       c06dd500 c010f6cb 00000001 c06dd0e0 df64d500 00200002 00200002 00200002 
       decbbd5c dec9951c 00000001 c010e4c5 00000000 00000001 dec99528 00000000 
Call Trace:
 [<c010fe42>] enqueue_task_casio+0x57/0xb2
 [<c010f6cb>] check_preempt_wakeup+0x4e/0x7d
 [<c010e4c5>] __wake_up_common+0x31/0x56
 [<c01a8271>] n_tty_receive_buf+0xc56/0xca1
 [<c01391a5>] __do_fault+0x274/0x2ae
 [<c0116c2b>] sysctl_string+0x86/0xdf
 [<c012ec8a>] sysctl_uts_string+0x83/0x8d
 [<c012ff55>] find_lock_page+0x12/0x5c
 [<c0130885>] filemap_fault+0x199/0x2ee
 [<c01a932b>] pty_write+0x2b/0x35
 [<c01a3816>] tty_default_put_char+0x15/0x18
 [<c0104e4c>] timer_interrupt+0x20/0x3c
 [<c012d475>] handle_IRQ_event+0x1a/0x3f
 [<c010e2f5>] enqueue_task+0xa/0x14
 [<c010f188>] activate_task+0x1c/0x27
 [<c010fc74>] sched_setscheduler+0x26d/0x2ca
 [<c010fd1c>] do_sched_setscheduler+0x4b/0x56
 [<c0102286>] sysenter_past_esp+0x5f/0x85
 =======================
Code: 10 c0 8d 46 0c 5e e9 cf f4 00 00 85 c0 74 05 e9 c8 ed 00 00 a1 00 c0 28 c0 c3 55 31 c9 57 89 c5 56 89 d7 53 89 c2 eb 20 8b 77 10 <8b> 48 10 8b 5f 0c 8b 50 0c 39 ce 77 0b 72 04 39 d3 73 05 8d 50 
EIP: [<c010e90b>] insert_casio_task_rb_tree+0x11/0x53 SS:ESP 0068:dedf9d8c
---[ end trace 1582eb3cf9b6aa96 ]---
BUG: unable to handle kernel NULL pointer dereference at virtual address 00000011
printing eip: c010e90b *pde = 00000000 
Oops: 0000 [#2] 
Modules linked in: ppdev ipv6 af_packet md_mod parport_pc lp parport pcnet32 mii intel_agp agpgart evdev ext3 jbd mbcache ide_cd cdrom ide_disk piix ide_core unix vga16fb cfbcopyarea vgastate cfbimgblt cfbfillrect

Pid: 6321, comm: casio_task Tainted: G      D (2.6.24-casio #1)
EIP: 0060:[<c010e90b>] EFLAGS: 00210002 CPU: 0
EIP is at insert_casio_task_rb_tree+0x11/0x53
EAX: 00000001 EBX: dfa3b0a0 ECX: 00000000 EDX: c06dd0e0
ESI: 000002c9 EDI: dfa3b0a0 EBP: c06dd0e0 ESP: dec63d8c
 DS: 007b ES: 007b FS: 0000 GS: 0033 SS: 0068
Process casio_task (pid: 6321, ti=dec62000 task=dede3500 task.ti=dec62000)
Stack: dfa3b0a0 dede3500 c06dd0e0 00000001 c010fe42 c06dd540 de893960 dede3500 
       c06dd500 c010f6cb 00000001 c06dd0e0 df64d500 00200002 00200002 00200002 
       decbbd5c dec9951c 00000001 c010e4c5 00000000 00000001 dec99528 00000000 
Call Trace:
 [<c010fe42>] enqueue_task_casio+0x57/0xb2
 [<c010f6cb>] check_preempt_wakeup+0x4e/0x7d
 [<c010e4c5>] __wake_up_common+0x31/0x56
 [<c01a8271>] n_tty_receive_buf+0xc56/0xca1
 [<c01391a5>] __do_fault+0x274/0x2ae
 [<c0116c2b>] sysctl_string+0x86/0xdf
 [<c012ec8a>] sysctl_uts_string+0x83/0x8d
 [<c012ff55>] find_lock_page+0x12/0x5c
 [<c0130885>] filemap_fault+0x199/0x2ee
 [<c01a932b>] pty_write+0x2b/0x35
 [<c01a3816>] tty_default_put_char+0x15/0x18
 [<c01a72ea>] opost+0x1b8/0x1ce
 [<c010e2f5>] enqueue_task+0xa/0x14
 [<c010f188>] activate_task+0x1c/0x27
 [<c010fc74>] sched_setscheduler+0x26d/0x2ca
 [<c010fd1c>] do_sched_setscheduler+0x4b/0x56
 [<c0102286>] sysenter_past_esp+0x5f/0x85
 =======================
Code: 10 c0 8d 46 0c 5e e9 cf f4 00 00 85 c0 74 05 e9 c8 ed 00 00 a1 00 c0 28 c0 c3 55 31 c9 57 89 c5 56 89 d7 53 89 c2 eb 20 8b 77 10 <8b> 48 10 8b 5f 0c 8b 50 0c 39 ce 77 0b 72 04 39 d3 73 05 8d 50 
EIP: [<c010e90b>] insert_casio_task_rb_tree+0x11/0x53 SS:ESP 0068:dec63d8c
---[ end trace 1582eb3cf9b6aa96 ]---
BUG: unable to handle kernel paging request at virtual address 0000dfa3
printing eip: c018d344 *pde = 00000000 
Oops: 0000 [#3] 
Modules linked in: ppdev ipv6 af_packet md_mod parport_pc lp parport pcnet32 mii intel_agp agpgart evdev ext3 jbd mbcache ide_cd cdrom ide_disk piix ide_core unix vga16fb cfbcopyarea vgastate cfbimgblt cfbfillrect

Pid: 6319, comm: casio_system Tainted: G      D (2.6.24-casio #1)
EIP: 0060:[<c018d344>] EFLAGS: 00210006 CPU: 0
EIP is at rb_insert_color+0x50/0xad
EAX: dfa3b1c2 EBX: dfa3b1c0 ECX: dfa3b1c0 EDX: 0000dfa3
ESI: dfa3b1c0 EDI: dfa3b1c0 EBP: dec4e000 ESP: dec4fcfc
 DS: 007b ES: 007b FS: 0000 GS: 0033 SS: 0068
Process casio_system (pid: 6319, ti=dec4e000 task=dede2000 task.ti=dec4e000)
Stack: c06dd0e0 dfa3b1c0 dede3a40 c06dd0e0 dec4e000 c010fe42 001280d2 00200000 
       00000000 0001b706 c06dd540 dede2140 c06dd540 c010f505 c06dd110 6b6c24ef 
       0000001f c06dd110 c06dd540 dede2140 dede2000 c010f953 c06dd110 c06dd540 
Call Trace:
 [<c010fe42>] enqueue_task_casio+0x57/0xb2
 [<c010f505>] update_curr+0x52/0xc4
 [<c010f953>] dequeue_entity+0xb/0x2c
 [<c010f987>] dequeue_task_fair+0x13/0x27
 [<c0226c70>] schedule+0x25f/0x27f
 [<c018d37e>] rb_insert_color+0x8a/0xad
 [<c010f8a1>] enqueue_entity+0x2b/0x3d
 [<c010e4c5>] __wake_up_common+0x31/0x56
 [<c01a8271>] n_tty_receive_buf+0xc56/0xca1
 [<c0104e4c>] timer_interrupt+0x20/0x3c
 [<c012d475>] handle_IRQ_event+0x1a/0x3f
 [<c012e71f>] handle_edge_irq+0xb1/0xce
 [<c010e2f5>] enqueue_task+0xa/0x14
 [<c010f188>] activate_task+0x1c/0x27
 [<c010f1c7>] try_to_wake_up+0x34/0x5b
 [<c0119f42>] __group_send_sig_info+0x64/0x6f
 [<c011a949>] group_send_sig_info+0x36/0x44
 [<c011ab69>] sys_kill+0xf8/0x104
 [<c0148fe0>] vfs_write+0xfb/0x10b
 [<c014966a>] sys_write+0x41/0x67
 [<c0102286>] sysenter_past_esp+0x5f/0x85
 =======================
Code: 8b 6b 04 39 fd 75 0e 8b 14 24 89 d8 89 df e8 78 ff ff ff 89 e9 83 09 01 89 f0 83 26 fe 8b 14 24 e8 16 ff ff ff eb 3e 85 d2 74 15 <8b> 02 a8 01 75 0f 83 c8 01 89 f7 89 02 83 0b 01 83 26 fe eb 25 
EIP: [<c018d344>] rb_insert_color+0x50/0xad SS:ESP 0068:dec4fcfc
---[ end trace 1582eb3cf9b6aa96 ]---
casio_open
casio_read
casio_read
casio_read
casio_read
casio_read
casio_read
casio_read
casio_read
casio_read
casio_read
casio_read
casio_read
casio_release
BUG: unable to handle kernel paging request at virtual address b1c00010
printing eip: c010e90b *pde = 00000000 
Oops: 0000 [#4] 
Modules linked in: ppdev ipv6 af_packet md_mod parport_pc lp parport pcnet32 mii intel_agp agpgart evdev ext3 jbd mbcache ide_cd cdrom ide_disk piix ide_core unix vga16fb cfbcopyarea vgastate cfbimgblt cfbfillrect

Pid: 6383, comm: casio_task Tainted: G      D (2.6.24-casio #1)
EIP: 0060:[<c010e90b>] EFLAGS: 00210086 CPU: 0
EIP is at insert_casio_task_rb_tree+0x11/0x53
EAX: b1c00000 EBX: 31fbd33f ECX: dfa3b1c0 EDX: dfa3b1c4
ESI: 00000300 EDI: dfa3b5a0 EBP: c06dd0e0 ESP: dec73d8c
 DS: 007b ES: 007b FS: 0000 GS: 0033 SS: 0068
Process casio_task (pid: 6383, ti=dec72000 task=dede3500 task.ti=dec72000)
Stack: dfa3b5a0 dede3500 c06dd0e0 00000001 c010fe42 c06dd540 de893960 dede3500 
       c06dd500 c010f6cb 00000001 c06dd0e0 df64d500 00200002 00200002 00200002 
       decbbd5c dec9951c 00000001 c010e4c5 00000000 00000001 dec99528 00000000 
Call Trace:
 [<c010fe42>] enqueue_task_casio+0x57/0xb2
 [<c010f6cb>] check_preempt_wakeup+0x4e/0x7d
 [<c010e4c5>] __wake_up_common+0x31/0x56
 [<c01a8271>] n_tty_receive_buf+0xc56/0xca1
 [<c01391a5>] __do_fault+0x274/0x2ae
 [<c0116c2b>] sysctl_string+0x86/0xdf
 [<c012ec8a>] sysctl_uts_string+0x83/0x8d
 [<c012ff55>] find_lock_page+0x12/0x5c
 [<c0130885>] filemap_fault+0x199/0x2ee
 [<c01a932b>] pty_write+0x2b/0x35
 [<c01a3816>] tty_default_put_char+0x15/0x18
 [<c01a72ea>] opost+0x1b8/0x1ce
 [<c010e2f5>] enqueue_task+0xa/0x14
 [<c010f188>] activate_task+0x1c/0x27
 [<c010fc74>] sched_setscheduler+0x26d/0x2ca
 [<c010fd1c>] do_sched_setscheduler+0x4b/0x56
 [<c0102286>] sysenter_past_esp+0x5f/0x85
 =======================
Code: 10 c0 8d 46 0c 5e e9 cf f4 00 00 85 c0 74 05 e9 c8 ed 00 00 a1 00 c0 28 c0 c3 55 31 c9 57 89 c5 56 89 d7 53 89 c2 eb 20 8b 77 10 <8b> 48 10 8b 5f 0c 8b 50 0c 39 ce 77 0b 72 04 39 d3 73 05 8d 50 
EIP: [<c010e90b>] insert_casio_task_rb_tree+0x11/0x53 SS:ESP 0068:dec73d8c
---[ end trace 1582eb3cf9b6aa96 ]---
BUG: unable to handle kernel paging request at virtual address 00dfa3c1
printing eip: c010e90b *pde = 00000000 
Oops: 0000 [#5] 
Modules linked in: ppdev ipv6 af_packet md_mod parport_pc lp parport pcnet32 mii intel_agp agpgart evdev ext3 jbd mbcache ide_cd cdrom ide_disk piix ide_core unix vga16fb cfbcopyarea vgastate cfbimgblt cfbfillrect

Pid: 6384, comm: casio_task Tainted: G      D (2.6.24-casio #1)
EIP: 0060:[<c010e90b>] EFLAGS: 00210006 CPU: 0
EIP is at insert_casio_task_rb_tree+0x11/0x53
EAX: 00dfa3b1 EBX: a9d7a1be ECX: dfa3b1bf EDX: dfa3b1c7
ESI: 00000300 EDI: dfa3bca0 EBP: c06dd0e0 ESP: de8c1d8c
 DS: 007b ES: 007b FS: 0000 GS: 0033 SS: 0068
Process casio_task (pid: 6384, ti=de8c0000 task=deb87500 task.ti=de8c0000)
Stack: dfa3bca0 deb87500 c06dd0e0 00000001 c010fe42 c06dd540 de893960 deb87500 
       c06dd500 c010f6cb 00000001 c06dd0e0 df64d500 00200002 00200002 00200002 
       decbbd5c dec9951c 00000001 c010e4c5 00000000 00000001 dec99528 00000000 
Call Trace:
 [<c010fe42>] enqueue_task_casio+0x57/0xb2
 [<c010f6cb>] check_preempt_wakeup+0x4e/0x7d
 [<c010e4c5>] __wake_up_common+0x31/0x56
 [<c01a8271>] n_tty_receive_buf+0xc56/0xca1
 [<c01391a5>] __do_fault+0x274/0x2ae
 [<c0116c2b>] sysctl_string+0x86/0xdf
 [<c012ec8a>] sysctl_uts_string+0x83/0x8d
 [<c012ff55>] find_lock_page+0x12/0x5c
 [<c0130885>] filemap_fault+0x199/0x2ee
 [<c01a932b>] pty_write+0x2b/0x35
 [<c01a3816>] tty_default_put_char+0x15/0x18
 [<c01a72ea>] opost+0x1b8/0x1ce
 [<c010e2f5>] enqueue_task+0xa/0x14
 [<c010f188>] activate_task+0x1c/0x27
 [<c010fc74>] sched_setscheduler+0x26d/0x2ca
 [<c010fd1c>] do_sched_setscheduler+0x4b/0x56
 [<c0102286>] sysenter_past_esp+0x5f/0x85
 =======================
Code: 10 c0 8d 46 0c 5e e9 cf f4 00 00 85 c0 74 05 e9 c8 ed 00 00 a1 00 c0 28 c0 c3 55 31 c9 57 89 c5 56 89 d7 53 89 c2 eb 20 8b 77 10 <8b> 48 10 8b 5f 0c 8b 50 0c 39 ce 77 0b 72 04 39 d3 73 05 8d 50 
EIP: [<c010e90b>] insert_casio_task_rb_tree+0x11/0x53 SS:ESP 0068:de8c1d8c
---[ end trace 1582eb3cf9b6aa96 ]---
BUG: unable to handle kernel paging request at virtual address b1c00010
printing eip: c010e90b *pde = 00000000 
Oops: 0000 [#6] 
Modules linked in: ppdev ipv6 af_packet md_mod parport_pc lp parport pcnet32 mii intel_agp agpgart evdev ext3 jbd mbcache ide_cd cdrom ide_disk piix ide_core unix vga16fb cfbcopyarea vgastate cfbimgblt cfbfillrect

Pid: 6385, comm: casio_task Tainted: G      D (2.6.24-casio #1)
EIP: 0060:[<c010e90b>] EFLAGS: 00210086 CPU: 0
EIP is at insert_casio_task_rb_tree+0x11/0x53
EAX: b1c00000 EBX: 5d7f32f9 ECX: dfa3b1c0 EDX: dfa3b1c4
ESI: 00000301 EDI: dfa3b5a0 EBP: c06dd0e0 ESP: de8ddd8c
 DS: 007b ES: 007b FS: 0000 GS: 0033 SS: 0068
Process casio_task (pid: 6385, ti=de8dc000 task=de8da000 task.ti=de8dc000)
Stack: dfa3b5a0 de8da000 c06dd0e0 00000001 c010fe42 c06dd540 de893960 de8da000 
       c06dd500 c010f6cb 00000001 c06dd0e0 df64d500 00200002 00200002 00200002 
       decbbd5c dec9951c 00000001 c010e4c5 00000000 00000001 dec99528 00000000 
Call Trace:
 [<c010fe42>] enqueue_task_casio+0x57/0xb2
 [<c010f6cb>] check_preempt_wakeup+0x4e/0x7d
 [<c010e4c5>] __wake_up_common+0x31/0x56
 [<c01a8271>] n_tty_receive_buf+0xc56/0xca1
 [<c01391a5>] __do_fault+0x274/0x2ae
 [<c0116c2b>] sysctl_string+0x86/0xdf
 [<c012ec8a>] sysctl_uts_string+0x83/0x8d
 [<c012ff55>] find_lock_page+0x12/0x5c
 [<c0130885>] filemap_fault+0x199/0x2ee
 [<c01a932b>] pty_write+0x2b/0x35
 [<c01a3816>] tty_default_put_char+0x15/0x18
 [<c01a72ea>] opost+0x1b8/0x1ce
 [<c010e2f5>] enqueue_task+0xa/0x14
 [<c010f188>] activate_task+0x1c/0x27
 [<c010fc74>] sched_setscheduler+0x26d/0x2ca
 [<c010fd1c>] do_sched_setscheduler+0x4b/0x56
 [<c0102286>] sysenter_past_esp+0x5f/0x85
 =======================
Code: 10 c0 8d 46 0c 5e e9 cf f4 00 00 85 c0 74 05 e9 c8 ed 00 00 a1 00 c0 28 c0 c3 55 31 c9 57 89 c5 56 89 d7 53 89 c2 eb 20 8b 77 10 <8b> 48 10 8b 5f 0c 8b 50 0c 39 ce 77 0b 72 04 39 d3 73 05 8d 50 
EIP: [<c010e90b>] insert_casio_task_rb_tree+0x11/0x53 SS:ESP 0068:de8ddd8c
---[ end trace 1582eb3cf9b6aa96 ]---
BUG: unable to handle kernel paging request at virtual address a3b1c010
printing eip: c010e90b *pde = 00000000 
Oops: 0000 [#7] 
Modules linked in: ppdev ipv6 af_packet md_mod parport_pc lp parport pcnet32 mii intel_agp agpgart evdev ext3 jbd mbcache ide_cd cdrom ide_disk piix ide_core unix vga16fb cfbcopyarea vgastate cfbimgblt cfbfillrect

Pid: 6386, comm: casio_task Tainted: G      D (2.6.24-casio #1)
EIP: 0060:[<c010e90b>] EFLAGS: 00210086 CPU: 0
EIP is at insert_casio_task_rb_tree+0x11/0x53
EAX: a3b1c000 EBX: d53fca45 ECX: dfa3b1bd EDX: dfa3b1c5
ESI: 00000301 EDI: dfa3bca0 EBP: c06dd0e0 ESP: de8c7d8c
 DS: 007b ES: 007b FS: 0000 GS: 0033 SS: 0068
Process casio_task (pid: 6386, ti=de8c6000 task=de8da540 task.ti=de8c6000)
Stack: dfa3bca0 de8da540 c06dd0e0 00000001 c010fe42 c06dd540 de893960 de8da540 
       c06dd500 c010f6cb 00000001 c06dd0e0 df64d500 00200002 00200002 00200002 
       decbbd5c dec9951c 00000001 c010e4c5 00000000 00000001 dec99528 00000000 
Call Trace:
 [<c010fe42>] enqueue_task_casio+0x57/0xb2
 [<c010f6cb>] check_preempt_wakeup+0x4e/0x7d
 [<c010e4c5>] __wake_up_common+0x31/0x56
 [<c01a8271>] n_tty_receive_buf+0xc56/0xca1
 [<c01391a5>] __do_fault+0x274/0x2ae
 [<c0116c2b>] sysctl_string+0x86/0xdf
 [<c012ec8a>] sysctl_uts_string+0x83/0x8d
 [<c012ff55>] find_lock_page+0x12/0x5c
 [<c0130885>] filemap_fault+0x199/0x2ee
 [<c01a932b>] pty_write+0x2b/0x35
 [<c01a3816>] tty_default_put_char+0x15/0x18
 [<c01a72ea>] opost+0x1b8/0x1ce
 [<c010e2f5>] enqueue_task+0xa/0x14
 [<c010f188>] activate_task+0x1c/0x27
 [<c010fc74>] sched_setscheduler+0x26d/0x2ca
 [<c010fd1c>] do_sched_setscheduler+0x4b/0x56
 [<c0102286>] sysenter_past_esp+0x5f/0x85
 =======================
Code: 10 c0 8d 46 0c 5e e9 cf f4 00 00 85 c0 74 05 e9 c8 ed 00 00 a1 00 c0 28 c0 c3 55 31 c9 57 89 c5 56 89 d7 53 89 c2 eb 20 8b 77 10 <8b> 48 10 8b 5f 0c 8b 50 0c 39 ce 77 0b 72 04 39 d3 73 05 8d 50 
EIP: [<c010e90b>] insert_casio_task_rb_tree+0x11/0x53 SS:ESP 0068:de8c7d8c
---[ end trace 1582eb3cf9b6aa96 ]---
