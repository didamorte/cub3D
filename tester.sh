#!/bin/bash

EXECUTABLE="./cub3D"

echo "=========================================="
echo "  Testing INVALID maps with Valgrind"
echo "=========================================="
echo ""

for map in maps/invalid/*.cub maps/invalid/*.cube; do
	if [ -f "$map" ]; then
		echo "Testing: $map"
		echo "------------------------------------------"
		valgrind --leak-check=full --show-leak-kinds=all $EXECUTABLE "$map" 2>&1 | grep -E "(ERROR SUMMARY|definitely lost|indirectly lost|possibly lost|still reachable|Error)"
		echo ""
	fi
done

echo "=========================================="
echo "  Testing VALID maps with Valgrind"
echo "=========================================="
echo ""

for map in maps/valid/*.cub; do
	if [ -f "$map" ]; then
		echo "Testing: $map"
		echo "------------------------------------------"
		timeout 2 valgrind --leak-check=full --show-leak-kinds=all $EXECUTABLE "$map" 2>&1 | grep -E "(ERROR SUMMARY|definitely lost|indirectly lost|possibly lost|still reachable|Error)"
		echo ""
	fi
done

echo "=========================================="
echo "  All maps tested"
echo "=========================================="
