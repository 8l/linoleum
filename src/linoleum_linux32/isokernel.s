/*
 *	linoleum_linux32 Linoleum Run-Time Module for linux 32-bit systems
 *	Copyright (C) 2004-2006 Peterpaul Klein Haneveld
 *
 *	This program is free software ;  you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation ;  either version 2
 *	of the License, or (at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program	;  if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

	.text
	.globl isokernel
	.type isokernel, @function
isokernel:
	movl	%eax, aAtExit
	movl	%ebx, bAtExit
	movl	%ecx, cAtExit
	movl	%edx, dAtExit
	movl	%esi, eAtExit
	/* movl	%ebp, xAtExit */
	pusha
	call	ISOKRNLCALL
	popa
	movl	pWorkspace, %edi
	/* set %ebp with result of call */
	cmpl	$0, isostatus
	je	.isokernel_end
	movl	FAIL, %ebp
	jmp	.isokernel_exit
.isokernel_end:
	movl	DONE, %ebp
.isokernel_exit:
	ret
.size isokernel,. - isokernel


	.text
	.globl linoleum
	.type linoleum, @function
linoleum:
	pushl	%ebp
	movl	%esp, sAtEntry
	movl	pWorkspace, %edi
	xorl	%eax, %eax
	xorl	%ebx, %ebx
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	xorl	%esi, %esi
	xorl	%ebp, %ebp
	call	*pCodeEntry
	movl	%eax, aAtExit
	movl	%ebx, bAtExit
	movl	%ecx, cAtExit
	movl	%edx, dAtExit
	movl	%esi, eAtExit
	movl	%ebp, xAtExit
	movl	sAtEntry, %esp
	popl	%ebp
	ret
.size linoleum,. - linoleum
