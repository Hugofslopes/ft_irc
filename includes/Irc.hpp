/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfilipe- <hfilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:58:24 by hfilipe-          #+#    #+#             */
/*   Updated: 2025/07/16 11:46:13 by hfilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <vector>
# include <ctime>

# include "Client.hpp"
# include "Input.hpp"
# include "Channel.hpp"

namespace Irc{
    std::vector<Client>     clients;
    std::vector<Channel>    channels;
    Input                   input;
    std::string             network_name;
    std::string             version;
    double                  time;

    std::string    getDateTime();
}
# endif