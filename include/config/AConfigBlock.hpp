#ifndef ACONFIGBLOCK_HPP
# define ACONFIGBLOCK_HPP

#include	<vector>
#include	<string>

/**
 * @class AConfigBlock
 * @brief Abstract base class for configuration blocks.
 *
 * This class represents a configuration block that can contain other configuration blocks.
 * It provides methods to add blocks and print the configuration.
 */
class AConfigBlock {

    private:
        /**
         * @brief Default constructor.
         *
         * This constructor is private to prevent creating an instance without a name.
         */
        AConfigBlock(void);

    protected:
        std::string					_name;  ///< Name of the configuration block.
        std::vector<AConfigBlock *>	blocks; ///< Vector of pointers to child configuration blocks.

    public:
        /**
         * @brief Constructor with name.
         * @param name Name of the configuration block.
         */
        AConfigBlock(const std::string &name);

        /**
         * @brief Copy constructor.
         * @param other Another AConfigBlock object to copy from.
         */
        AConfigBlock(const AConfigBlock &other);

        /**
         * @brief Virtual destructor.
         */
        virtual ~AConfigBlock(void);

        /**
         * @brief Assignment operator.
         * @param other Another AConfigBlock object to assign from.
         * @return Reference to this object.
         */
        AConfigBlock &operator=(const AConfigBlock &other);

        /**
         * @brief Pure virtual function to print the configuration.
         * @param indent Indentation level for printing.
         */
        virtual void printConfig(int indent = 0) const = 0;

        /**
         * @brief Adds a new block to the configuration.
         * @param newBlock Pointer to the new configuration block to add.
         */
        virtual void addBlock(AConfigBlock *newBlock);
};

#endif