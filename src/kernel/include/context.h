
#pragma once

struct context {
	void* sp;
	void* ra;
	void* s0;
	void* s1;
	void* s2;
	void* s3;
	void* s4;
	void* s5;
	void* s6;
	void* s7;
	void* s8;
	void* s9;
	void* s10;
	void* s11;
	void* satp;
	void* sepc;
};


void ctx_sw(struct context *previous, struct context *next);
