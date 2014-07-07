#!/bin/bash
python -c "print unicode('$1'.decode('hex'), 'utf-16-be').encode('utf8')"