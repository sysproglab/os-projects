#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <asm/irq_vectors.h>

// ������� ��������� ���� ������� �������
irq_handler_t irq_handler (int irq, void *dev_id, struct pt_regs *regs)
{
	static unsigned char scancode;

	// ���������� ��� ��������� ���������� ����
	scancode = inb(0x60);

  // ����� ��������� �������
	printk(KERN_INFO "Scan Code %u\n",(unsigned int)scancode);
	return (irq_handler_t) IRQ_HANDLED;
}

// ������������� ������
int init_module ()
{
	// ������ ���������� IRQ 1 (���������� ����������)
	// ���������� ��� ����, ��� �� ������� �� ��� ����������
	return request_irq (1, (irq_handler_t) irq_handler, IRQF_SHARED, "test_keyboard_irq_handler", (void *)(irq_handler));
}

// �������� ������
void cleanup_module()
{
	// ����������� ����������� ����������
	free_irq(1, NULL);
}
