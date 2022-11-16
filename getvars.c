#include "shellvars.h"

/**
 * getspecial - wrapper for getspecial static var
 * Return: address of shellvar special
 */

ShellVar **getspecial()
{
	static ShellVar *special;

	return (&special);
}
/**
 * getsvars - get static vars wrapper
 *
 * Return: address of var
 */
ShellVar **getvars()
{
	static ShellVar *vars;

	return (&vars);
}
