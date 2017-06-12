#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <asm/irq_vectors.h>

// Функция получения кода нажатой клавиши
irq_handler_t irq_handler (int irq, void *dev_id, struct pt_regs *regs)
{
	static unsigned char scancode;

	// Прерывание для получения считанного кода
	scancode = inb(0x60);

  // Вывод считанной клавиши
	printk(KERN_INFO "Scan Code %u\n",(unsigned int)scancode);
	return (irq_handler_t) IRQ_HANDLED;
}

// Инициализация модуля
int init_module ()
{
	// Запрос прерывания IRQ 1 (прерывание клавиатуры)
	// Необходимо для того, что бы перейти на наш обработчик
	return request_irq (1, (irq_handler_t) irq_handler, IRQF_SHARED, "test_keyboard_irq_handler", (void *)(irq_handler));
}

// Выгрузка модуля
void cleanup_module()
{
	// Освобождаем запрошенное прерывание
	free_irq(1, NULL);
}
