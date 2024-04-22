#pragma once

namespace yk {
	//笔者使用北通蝙蝠2的手柄进行测试的
	class Joystick {
	public:
		enum class EKey {
			kA = 0,
			kB = 1,
			kX = 2,
			kY = 3,
			kLB = 4,
			kRB = 5,
			kBack = 6,
			kStart = 7,
			kLHandle = 8, //左侧摇杆摁下才触发
			kRHandle = 9, //右侧摇杆摁下才触发
			kDirectionUp = 10,
			kDirectionRight = 11,
			kkDirectionDown = 12,
			kDirectionLeft = 13,
		};
	public:
		Joystick();
		virtual ~Joystick();

	};

}