@echo off
title=Window Buildscript
pushd ..\
call vendors\premake\premake5.exe vs2022
popd

IF %ERRORLEVEL% NEQ 0 (
	PAUSE
)