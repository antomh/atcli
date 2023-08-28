OUTPUT_PATH := ./output
CLI_NAME := atcli
TEST_NAME := test_server
CC := gcc
CCPARAM := -std=c11

#------------------------------------------------------------------------------
# CLIENT+TEST
all: $(CLI_NAME) $(TEST_NAME)

$(OUTPUT_PATH):
	@mkdir $(OUTPUT_PATH)

#------------------------------------------------------------------------------
# CLIENT

$(CLI_NAME): clean $(CLI_NAME).o tc.o
	@echo "Build binary file.\n"
	@$(CC)                           \
		$(CCPARAM)                   \
		$(OUTPUT_PATH)/$(CLI_NAME).o \
		$(OUTPUT_PATH)/tc.o          \
		-o $(OUTPUT_PATH)/$@
	@echo "Compilation is successfully over!\n"

$(CLI_NAME).o: $(OUTPUT_PATH) $(CLI_NAME).c
	@echo "Build object file.\n"
	@$(CC)                   \
		$(CCPARAM)           \
		-c $(CLI_NAME).c     \
		-o $(OUTPUT_PATH)/$@

run: $(CLI_NAME)
	@$(OUTPUT_PATH)/$(CLI_NAME)

clean:
	@rm -f                         \
		$(OUTPUT_PATH)/$(CLI_NAME) \
		$(OUTPUT_PATH)/$(CLI_NAME).o

tc.o: $(OUTPUT_PATH) tc.c
	@$(CC)                   \
		$(CCPARAM)           \
		-c tc.c              \
		-o $(OUTPUT_PATH)/$@

#------------------------------------------------------------------------------
# TEST

$(TEST_NAME): test_clean $(TEST_NAME).o
	@echo "Build binary test file.\n"
	@$(CC)                            \
		$(CCPARAM)                    \
		$(OUTPUT_PATH)/$(TEST_NAME).o \
		-o $(OUTPUT_PATH)/$@
	@echo "Compilation of test is successfully over!\n"

$(TEST_NAME).o: $(OUTPUT_PATH) $(TEST_NAME).c
	@echo "Build object test file.\n"
	@$(CC)                \
		$(CCPARAM)        \
		-c $(TEST_NAME).c \
		-o $(OUTPUT_PATH)/$@

test_run: $(TEST_NAME)
	@$(OUTPUT_PATH)/$(TEST_NAME)

test_clean:
	@rm -f                          \
		$(OUTPUT_PATH)/$(TEST_NAME) \
		$(OUTPUT_PATH)/$(TEST_NAME).o