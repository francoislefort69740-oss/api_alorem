--
-- PostgreSQL database dump
--

\restrict dUHdlgyI5EpuyVklnBBlW7WFquoha7EgdWns1mdZwj3qk9rxF3h3BftBzWMDsK1

-- Dumped from database version 18.6
-- Dumped by pg_dump version 18.6

-- Started on 2026-10-07 15:57:09

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 220 (class 1259 OID 16399)
-- Name: commande; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.commande (
    uid integer NOT NULL,
    title character varying(255),
    serial_number integer,
    current_step integer DEFAULT 0 NOT NULL,
    currently_going_on boolean DEFAULT false NOT NULL,
    loaded boolean DEFAULT false NOT NULL
);


ALTER TABLE public.commande OWNER TO postgres;

--
-- TOC entry 221 (class 1259 OID 16413)
-- Name: commande_step0; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.commande_step0 (
    commande_uid integer NOT NULL,
    reference integer NOT NULL,
    report_number integer NOT NULL,
    customer character varying(255) NOT NULL,
    customer_serial_number character varying(255) NOT NULL,
    serial_number_alorem character varying(255) NOT NULL,
    type character varying(100) NOT NULL
);


ALTER TABLE public.commande_step0 OWNER TO postgres;

--
-- TOC entry 222 (class 1259 OID 16432)
-- Name: commande_step1; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.commande_step1 (
    commande_uid integer NOT NULL,
    reference integer NOT NULL,
    fabrication_plate_adr boolean,
    alorem_plate boolean,
    booklet_pouch boolean,
    user_manual boolean,
    instruction_of_use boolean,
    certificates_adr boolean,
    grounding_adhesive boolean,
    conformity_certificate_marking boolean
);


ALTER TABLE public.commande_step1 OWNER TO postgres;

--
-- TOC entry 223 (class 1259 OID 16444)
-- Name: commande_step2; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.commande_step2 (
    commande_uid integer NOT NULL,
    reference integer NOT NULL,
    marque_principale character varying(255) NOT NULL,
    tare integer NOT NULL,
    material character varying(100) NOT NULL,
    capacity20 integer NOT NULL,
    gross_mass integer NOT NULL,
    fabrication_date character varying(50) NOT NULL,
    shell_thickness integer NOT NULL,
    pictogram_stacking boolean,
    weight_stacking integer NOT NULL
);


ALTER TABLE public.commande_step2 OWNER TO postgres;

--
-- TOC entry 224 (class 1259 OID 16464)
-- Name: commande_step3; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.commande_step3 (
    commande_uid integer NOT NULL,
    reference integer NOT NULL,
    bottom_retention_face integer,
    bottom_retention_right integer,
    bottom_retention_left integer,
    bottom_retention_behind integer,
    upper_retention integer,
    lifting_rings integer,
    forklift_pass integer,
    dashboard integer,
    unauthorized_repair boolean NOT NULL
);


ALTER TABLE public.commande_step3 OWNER TO postgres;

--
-- TOC entry 225 (class 1259 OID 16477)
-- Name: commande_step4; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.commande_step4 (
    commande_uid integer NOT NULL,
    reference integer NOT NULL,
    internal_na boolean NOT NULL,
    internal_ok boolean,
    internal_clean boolean NOT NULL,
    internal_object_inside boolean NOT NULL,
    internal_pollution boolean NOT NULL
);


ALTER TABLE public.commande_step4 OWNER TO postgres;

--
-- TOC entry 226 (class 1259 OID 16493)
-- Name: commande_step5; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.commande_step5 (
    commande_uid integer NOT NULL,
    reference integer NOT NULL,
    epaisseur_na boolean NOT NULL,
    epaisseur_min_side_front integer NOT NULL,
    epaisseur_min_side_back integer NOT NULL,
    epaisseur_min_side_right integer NOT NULL,
    epaisseur_min_side_left integer NOT NULL,
    epaisseur_side_front_result1 integer NOT NULL,
    epaisseur_side_front_result2 integer NOT NULL,
    epaisseur_side_front_result3 integer NOT NULL,
    epaisseur_side_front_result4 integer NOT NULL,
    epaisseur_side_front_result5 integer NOT NULL,
    epaisseur_side_back_result1 integer NOT NULL,
    epaisseur_side_back_result2 integer NOT NULL,
    epaisseur_side_back_result3 integer NOT NULL,
    epaisseur_side_back_result4 integer NOT NULL,
    epaisseur_side_back_result5 integer NOT NULL,
    epaisseur_side_right_result1 integer NOT NULL,
    epaisseur_side_right_result2 integer NOT NULL,
    epaisseur_side_right_result3 integer NOT NULL,
    epaisseur_side_right_result4 integer NOT NULL,
    epaisseur_side_right_result5 integer NOT NULL,
    epaisseur_side_left_result1 integer NOT NULL,
    epaisseur_side_left_result2 integer NOT NULL,
    epaisseur_side_left_result3 integer NOT NULL,
    epaisseur_side_left_result4 integer NOT NULL,
    epaisseur_side_left_result5 integer NOT NULL
);


ALTER TABLE public.commande_step5 OWNER TO postgres;

--
-- TOC entry 227 (class 1259 OID 16530)
-- Name: commande_step6; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.commande_step6 (
    commande_uid integer NOT NULL,
    reference integer NOT NULL,
    etancheite_ok1 boolean,
    etancheite_date1 character varying(50) NOT NULL,
    etancheite_bar1 real NOT NULL,
    etancheite_ok2 boolean,
    etancheite_date2 character varying(50) NOT NULL,
    etancheite_bar2 real NOT NULL
);


ALTER TABLE public.commande_step6 OWNER TO postgres;

--
-- TOC entry 219 (class 1259 OID 16398)
-- Name: commande_uid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public.commande ALTER COLUMN uid ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public.commande_uid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- TOC entry 4888 (class 2606 OID 16410)
-- Name: commande commande_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande
    ADD CONSTRAINT commande_pkey PRIMARY KEY (uid);


--
-- TOC entry 4890 (class 2606 OID 16412)
-- Name: commande commande_serial_number_unique; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande
    ADD CONSTRAINT commande_serial_number_unique UNIQUE (serial_number);


--
-- TOC entry 4892 (class 2606 OID 16426)
-- Name: commande_step0 commande_step0_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande_step0
    ADD CONSTRAINT commande_step0_pkey PRIMARY KEY (commande_uid);


--
-- TOC entry 4894 (class 2606 OID 16438)
-- Name: commande_step1 commande_step1_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande_step1
    ADD CONSTRAINT commande_step1_pkey PRIMARY KEY (commande_uid);


--
-- TOC entry 4896 (class 2606 OID 16458)
-- Name: commande_step2 commande_step2_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande_step2
    ADD CONSTRAINT commande_step2_pkey PRIMARY KEY (commande_uid);


--
-- TOC entry 4898 (class 2606 OID 16471)
-- Name: commande_step3 commande_step3_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande_step3
    ADD CONSTRAINT commande_step3_pkey PRIMARY KEY (commande_uid);


--
-- TOC entry 4900 (class 2606 OID 16487)
-- Name: commande_step4 commande_step4_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande_step4
    ADD CONSTRAINT commande_step4_pkey PRIMARY KEY (commande_uid);


--
-- TOC entry 4902 (class 2606 OID 16524)
-- Name: commande_step5 commande_step5_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande_step5
    ADD CONSTRAINT commande_step5_pkey PRIMARY KEY (commande_uid);


--
-- TOC entry 4904 (class 2606 OID 16540)
-- Name: commande_step6 commande_step6_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande_step6
    ADD CONSTRAINT commande_step6_pkey PRIMARY KEY (commande_uid);


--
-- TOC entry 4905 (class 2606 OID 16427)
-- Name: commande_step0 commande_step0_commande_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande_step0
    ADD CONSTRAINT commande_step0_commande_fk FOREIGN KEY (commande_uid) REFERENCES public.commande(uid) ON DELETE CASCADE;


--
-- TOC entry 4906 (class 2606 OID 16439)
-- Name: commande_step1 commande_step1_commande_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande_step1
    ADD CONSTRAINT commande_step1_commande_fk FOREIGN KEY (commande_uid) REFERENCES public.commande(uid) ON DELETE CASCADE;


--
-- TOC entry 4907 (class 2606 OID 16459)
-- Name: commande_step2 commande_step2_commande_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande_step2
    ADD CONSTRAINT commande_step2_commande_fk FOREIGN KEY (commande_uid) REFERENCES public.commande(uid) ON DELETE CASCADE;


--
-- TOC entry 4908 (class 2606 OID 16472)
-- Name: commande_step3 commande_step3_commande_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande_step3
    ADD CONSTRAINT commande_step3_commande_fk FOREIGN KEY (commande_uid) REFERENCES public.commande(uid) ON DELETE CASCADE;


--
-- TOC entry 4909 (class 2606 OID 16488)
-- Name: commande_step4 commande_step4_commande_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande_step4
    ADD CONSTRAINT commande_step4_commande_fk FOREIGN KEY (commande_uid) REFERENCES public.commande(uid) ON DELETE CASCADE;


--
-- TOC entry 4910 (class 2606 OID 16525)
-- Name: commande_step5 commande_step5_commande_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande_step5
    ADD CONSTRAINT commande_step5_commande_fk FOREIGN KEY (commande_uid) REFERENCES public.commande(uid) ON DELETE CASCADE;


--
-- TOC entry 4911 (class 2606 OID 16541)
-- Name: commande_step6 commande_step6_commande_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.commande_step6
    ADD CONSTRAINT commande_step6_commande_fk FOREIGN KEY (commande_uid) REFERENCES public.commande(uid) ON DELETE CASCADE;


-- Completed on 2026-10-07 15:57:10

--
-- PostgreSQL database dump complete
--

\unrestrict dUHdlgyI5EpuyVklnBBlW7WFquoha7EgdWns1mdZwj3qk9rxF3h3BftBzWMDsK1

